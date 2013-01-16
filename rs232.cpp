//created: 23/10/12
//author: Simon Rankine
//updated: 22/11/12

#include "rs232.h"
#include "stdio.h"
#include "stdlib.h"
#include "ctype.h"
#include "error.h"
#include "string.h"
#include "circuit.h"

RS232::RS232(){

}

void RS232::establish(int pcBaud){
	//attempts to establish RS232 communication
	established=false;
	Serial1.begin(pcBaud);//establish serial link using defined baud rate
	char c;
	while(c != '&'){
		//wait for dummy serial establishment charector
		c = Serial1.read();
	}
	established = true;
	//trun on debug LED
	digitalWrite(13,HIGH);
	//send acknoledgement of connection
	Serial1.print('&');
}

void RS232::listen(){
	if(Serial1.available() > 0){//check for new serial data
		char c = Serial1.read();//read in serial data
		int R1,R2,R3,resonance,corner;
		float C1;
		String message;
		//decide what to do
		switch(c){
		case 'F':
			call(&motor::forwards,go,c);
			break;
		case 'B':
			call(&motor::backwards,go,c);
			break;
		case 'A':
			call(&motor::anticlockwise,go,c);
			break;
		case 'C':
			call(&motor::clockwise,go,c);
			break;
		case 'S':
			call(&motor::stop,go,c);
			break;
		case '&':
			Serial1.print("&");
			break;
		case 'f':
			call(&motor::run,go,c);
			break;
		case 'b':
			call(&motor::runBack,go,c);
			break;
		case 'v':
			error::verbose(true);
			break;
		case 'q':
			error::verbose(false);
			break;
		case '1':
			float port2;
			circuit::regulator(&port2,true);
			message = "V=";
			char str[10];
			dtostrf(port2,2,3,str);
			message += str;
			Serial1.print(message);
			break;
		case '2':
			circuit::divider(&R1,&R2,true);
			message = "R1=";
			message += R1;
			message += " R2=";
			message += R2;
			Serial1.print(message);
			break;
		case '3':
			circuit::delta(&R2,&R3,true);
			message = "R2=";
			message += R2;
			message += " R3=";
			message += R3;
			Serial1.print(message);
			break;
		case '4':
			circuit::star(&R1,&R2,true);
			message = "R1=";
			message += R1;
			message += " R2=";
			message += R2;
			Serial1.print(message);
			break;
		case '5':
			circuit::highPass(&R1,&C1,&corner,true);
			Serial1.print("R1=");
			Serial1.print(R1);
			Serial1.print(" C1=");
			Serial1.print(C1);
			Serial1.print("nF Corner=");
			Serial1.print(corner);
			Serial1.print("Hz");
			break;
		case '6':
			circuit::lowPass(&R1,&C1,&corner,true);
			Serial1.print("R1=");
			Serial1.print(R1);
			Serial1.print(" C1=");
			Serial1.print(C1);
			Serial1.print("nF Corner=");
			Serial1.print(corner);
			Serial1.print("Hz");
			break;
		case '7':
			circuit::bandPass(&R1,&C1,&resonance,true);
			Serial1.print("R1=");
			Serial1.print(R1);
			Serial1.print(" C1=");
			Serial1.print(C1);
			Serial1.print("nF Resonance=");
			Serial1.print(resonance);
			Serial1.print("kHz");
			break;
		case '8':
			circuit::disengauge();
			Serial1.print("#");
			break;
		case '9':
			int R1, R2, R3, R4;
			circuit::testFunction(&R1,&R2,&R3,&R4);
			Serial1.print(R1);
			Serial1.print(" ");
			Serial1.print(R2);
			break;
		default:
			error::set(2);
			break;
		}
	}
}

void RS232::call(boolean(motor::*member)(),motor go,char c){
	//handles motor functions with no steps
	Serial1.print("#");//acknoledge command
	Serial1.print(c);
	if((go.*member)()){//check for successful completion
		Serial1.print("*");//success
	}else{
		error::set(3);
	}
}

void RS232::call(boolean(motor::*member)(int steps),motor go,char c){
	char d;
	delay(100);
	if(Serial1.available() == 2){//check for xx numbers after command
		d = Serial1.read();//read in first digit
		if(isdigit(d)){//check is a valid digit
			int steps = atoi(&d) * 10;
			d = Serial1.read();//read in second digit
			if(isdigit(d)){
				steps = steps + atoi(&d);
				Serial1.print("#");
				Serial1.print(c);
				Serial1.print(steps);
				if((go.*member)(steps)){
					Serial1.print("*");
				}else{
					error::set(3);
				}
			}else{
				error::set(4);
			}
		}else{
			error::set(4);
		}
	}else if(Serial1.available() == 0){//individual command (i.e one step)
		Serial1.print("#");
		if((go.*member)(1)){
			Serial1.print(c);
			Serial1.print("*");
		}else{
			error::set(3);
		}
	}else{//invalid number of charectors
		Serial1.print("E");
		Serial1.flush();
	}
}