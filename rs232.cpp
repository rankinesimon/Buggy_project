//created: 23/10/12
//author: Simon Rankine
//updated: 05/11/12

#include "rs232.h"
#include "stdio.h"
#include "stdlib.h"
#include "ctype.h"
#include "error.h"
#include "string.h"

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
		case 'q':
			error::verbose(false);
		case '-1':
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
				int steps =+ atoi(&d);
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
			Serial1.print("*");
			Serial1.print(c);
		}else{
			error::set(3);
		}
	}else{//invalid number of charectors
		Serial1.print("E");
		Serial1.flush();
	}
}