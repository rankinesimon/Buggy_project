//created: 05/11/12
//author: Simon Rankine
//updated: 03/01/13

#include "circuit.h"
#include "Arduino.h"
#include "StopWatch.h"
#include <math.h>
const float E = 2.71828182846;
const int CALIB_R_HIGH = 988;
const int CALIB_R_LOW = 101;

bool circuit::regulator(float * port2, bool front){
	if(front){
		DC(3);
		ground(1);
	}else{
		DC(1);
		ground(3);
	}
	*port2 = readVoltage(2);
	return true;
}

bool circuit::divider(int * R1,int * R2, bool front){
	disengauge();
	if(front){
		*R1 = toR24(readR(3,2));
		disengauge();
		*R2 = toR24(readR(1,2));
		disengauge();
	}else{
		*R2 = toR24(readR(1,2));
		disengauge();
		*R1 = toR24(readR(3,2));
		disengauge();
	}
	
	return true;
}
bool circuit::delta(int * R2, int * R3, bool front){
	int R12, R23,c;
	float top,bottom,v1,v2;
	v1= 0;
	v2 = 0;
	disengauge();
	if(front){
		//set up relays for Ra
		disengauge();
		fiveV(3);
		ground(2);
		highZ(1);
		delay(500);
		//read Ra voltage
		for(c = 0; c < 5 ; c++){
			v1 += readVoltage(1);
			delay(100);
		}
		v1 = (float)v1/5;
		//calculate Ra
		*R2 = (5*1000-v1*1000)/v1;
		disengauge();
		//set up relatys for Rb
		fiveV(1);
		ground(3);
		highZ(2);
		delay(500);
		//read Rb voltage
		for(c = 0; c < 5 ; c++){
			v2 += readVoltage(2);
			delay(100);
		}
		//calculte
		v2 = (float)v2/5;
		//round and return
		*R3 = (1000*v2)/(5-v2);
		*R2 = toR24(*R2);
		*R3 = toR24(*R3);
		disengauge();
	}else{
		//set up relays for Ra
		disengauge();
		fiveV(1);
		ground(2);
		highZ(3);
		delay(500);
		//read Ra voltage
		for(c = 0; c < 5 ; c++){
			v1 += readVoltage(3);
			delay(100);
		}
		v1 = (float)v1/5;
		//calculate Ra
		*R2 = (5*1000-v1*1000)/v1;
		disengauge();
		//set up relatys for Rb
		fiveV(3);
		ground(1);
		highZ(2);
		delay(500);
		//read Rb voltage
		for(c = 0; c < 5 ; c++){
			v2 += readVoltage(2);
			delay(100);
		}
		//calculte
		v2 = (float)v2/5;
		//round and return
		*R3 = (1000*v2)/(5-v2);
		*R2 = toR24(*R2);
		*R3 = toR24(*R3);
		disengauge();
	}
	return true;
}

bool circuit::star(int * R1, int * R2, bool front){
	disengauge();
	int R;
	if(front){
		*R1 = readR(1,2);
		*R1 = toR24(*R1-1200);
		disengauge();
		*R2 = readR(3,2);
		*R2 = toR24(*R2-1200);
		disengauge();
	}else{
		*R1 = readR(3,2);
		*R1 = toR24(*R1 - 1200);
		disengauge();
		*R2 = readR(1,2);
		*R2 = toR24(*R2 - 1200);
		disengauge();
	}
	return true;
}
bool circuit::highPass(int * R1, float * C1, int * corner, bool front){
	float test;
	disengauge();
	if(front){
		*R1 = readR(3,2);
		disengauge();
		*C1 = readC(1,2);
		*corner = ((float)1/(float)(2 * PI * *R1 * *C1))*1000000000;
		disengauge();
	}else{
		*R1 = readR(1,2);
		disengauge();
		*C1 = readC(3,2);
		*corner = ((float)1/(float)(2 * PI * *R1 * *C1))*1000000000;
		disengauge();
	}
	*R1 = toR24(*R1);
	*C1 = toC12(*C1);
	return true;
}
bool circuit::lowPass(int * R1, float * C1, int * corner, bool front){
	disengauge();
	if(front){
		*R1 = readR(1,3);
		disengauge();
		*C1 = readC(3,2);
		*corner = ((float)1/(float)(2 * PI * *R1 * *C1))*1000000000;
		disengauge();
	}else{
		*R1 = readR(3,1);
		disengauge();
		*C1 = readC(1,2);
		*corner = ((float)1/(float)(2 * PI * *R1 * *C1))*1000000000;
		disengauge();
	}
	*R1 = toR24(*R1);
	*C1 = toC12(*C1);
	return true;
}
bool circuit::bandPass(int * R1, float * C1, int * resonance, bool front){
	float test;
	disengauge();
	if(front){
		*C1 = readC(1,2);
		disengauge();
		*R1 = readR(3,2);
		disengauge();
	}else{
		*C1 = readC(3,2);
		disengauge();
		*R1 = readR(1,2);
		disengauge();
	}
	test = (float)((float)1000/(float)(2 * PI * sqrt(*C1)));
	Serial1.print(test);
	*resonance = test;
	*R1 = toR24(*R1);
	//*C1 = toC12(*C1);
	return true;
}
bool circuit::testFunction(int *a, int *b, int *c, int *d){
	/*
     *This function operates in the same way
	 *as the ca functions, but instead of
	 *being specialised to one circuit it can
	 *be used for a wide range of testing purposes
	*/
	analogReference(DEFAULT);
	*a = readR(3,2);
	*b = readVoltage(3);
	*c = 0;
	*d = 0;
	return true;
}

bool circuit::disengauge(){
	/*
	 * This function disables all relays
	 * and should be used after every operation
	 * to ensure that relays are not accidentally
	 * left open at the start of the next operation.
	 * Faliure to use this function may result in 
	 * a short circuit!
	*/
	//disable all three output pin connectors
	highZ(1);
	highZ(2);
	highZ(3);
	//disconnect all ca components
	digitalWrite(33,LOW);
	digitalWrite(34,LOW);
	digitalWrite(35,LOW);
	digitalWrite(37,LOW);
	digitalWrite(39,LOW);
	digitalWrite(41,LOW);
}
float circuit::readR(int Rpin,int Gpin,bool noZ,bool low){
	float Vout = 0;
	int c;
	int Zpin = 6 - Rpin - Gpin;
	if(low){
		lowR(Rpin);
	}else{
		highR(Rpin);
	}
	ground(Gpin);
	if(noZ){
		ground(Zpin);
	}else{
		highZ(Zpin);
	}
	digitalWrite(41,HIGH);//enable A1 relay
	delay(500);
	for(c = 0; c < 5 ; c++){
		Vout += readVoltage(Rpin);
		delay(100);
	}
	Vout = (float)Vout/5;
	//Vout = readVoltage(Rpin);
	if (Vout == 1023){
		return 0;
	}
	if(low){
		float myR1 = (((float)CALIB_R_LOW*(float)5)/(5-Vout))-(float)CALIB_R_LOW;
		return myR1;
	}else{
		float myR1 = (((float)CALIB_R_HIGH*(float)5)/(5-Vout))-(float)CALIB_R_HIGH;
		if(myR1 < 300){
			return readR(Rpin,Gpin,noZ,true);
		}
		return myR1;
	}
	
}
float circuit::readC(int Rpin,int Gpin){
	int Zpin = 6 - Rpin - Gpin;
	//discharge capacitor
	highZ(Zpin);
	ground(Gpin);
	ground(Rpin);
	delay(1000);
	float threshold = 5 / E;
	StopWatch timer(StopWatch::MICROS);
	capR(Rpin);
	timer.start();
	float v = readVoltage(Rpin);
	while(v < threshold){
		v = readVoltage(Rpin);
		//wait for capcitor to charge
	}
	timer.stop();
	float time = timer.elapsed();
	return time/1000;
}
float circuit::readVoltage(int pin){
	if(pin == 2){
		return ((float)5*analogRead(A0))/(float)1023;
	}else if(pin == 1){
		return ((float)5*analogRead(A2))/(float)1023;
	}else{
		return ((float)5*analogRead(A3))/(float)1023;
	}
}
void circuit::caCon(int pin){
	if(pin == 1){
		digitalWrite(27,LOW);//p1 gnd
		digitalWrite(29,LOW);//p3 ca
		digitalWrite(25,HIGH);//p1 ca
	}else if(pin ==3){
		digitalWrite(31,LOW);//p3 gnd
		digitalWrite(25,LOW);//p1 ca
		digitalWrite(29,HIGH);//p3 ca
	}
}
void circuit::highR(int pin){
	if(pin == 1 || pin == 3){
		caCon(pin);
		digitalWrite(35,LOW);//low R
		digitalWrite(39,LOW);//freq
		digitalWrite(33,LOW);//10v
		digitalWrite(34,LOW);//5v
		digitalWrite(37,HIGH);//high R
	}
}
void circuit::lowR(int pin){
	if(pin == 1 || pin == 3){
		caCon(pin);
		digitalWrite(37,LOW);//high R
		digitalWrite(39,LOW);//freq
		digitalWrite(33,LOW);//10v
		digitalWrite(34,LOW);//5v
		digitalWrite(35,HIGH);//low R
	}
}
void circuit::DC(int pin){
	if(pin == 1 || pin == 3){
		caCon(pin);
		digitalWrite(35,LOW);//low R
		digitalWrite(37,LOW);//high R
		digitalWrite(39,LOW);//freq
		digitalWrite(34,LOW);//5v
		digitalWrite(33,HIGH);//10v
	}
}
void circuit::fiveV(int pin){
	if(pin == 1 || pin == 3){
		caCon(pin);
		digitalWrite(35,LOW);//low R
		digitalWrite(39,LOW);//freq
		digitalWrite(33,LOW);//10v
		digitalWrite(34,HIGH);//5v
		digitalWrite(37,LOW);//high R
	}
}
void circuit::capR(int pin){
	if(pin == 1 || pin == 3){
		caCon(pin);
		digitalWrite(35,LOW);//low R
		digitalWrite(37,LOW);//high R
		digitalWrite(33,LOW);//10v
		digitalWrite(34,LOW);//5v
		digitalWrite(39,HIGH);//freq
	}
}
void circuit::ground(int pin){
	if(pin == 1){
		digitalWrite(25,LOW);//p1 ca
		digitalWrite(27,HIGH);//p1 gnd
	}else if(pin == 2){
		digitalWrite(23,HIGH);//p2 gnd
	}else if(pin == 3){
		digitalWrite(29,LOW);//p3 ca
		digitalWrite(31,HIGH);//p3 gnd
	}
}
void circuit::highZ(int pin){
	if(pin == 1){
		digitalWrite(25,LOW);//p1 ca
		digitalWrite(27,LOW);//p1 gnd
	}else if(pin == 2){
		digitalWrite(23,LOW);//p2 gnd
	}else if(pin == 3){
		digitalWrite(29,LOW);//p3 ca
		digitalWrite(31,LOW);//p3 gnd
	}
}

int circuit::toR24(int R){

	//E24 Resistor series
	float E24[] = {
	20,22,24,27,30,33,36,39,43,47,51,56,62,68,75,82,91,100,
	110,120,130,150,160,180,200,220,240,270,300,330,360,
	390,430,470,510,560,620,680,750,820,910,1000,1100,1200,
	1300,1500,1600,1800,2000,2200,2400,2700,3000};

	return conform(R,E24,3000);
}
float circuit::toC12(int C){
	//E12 Capacitor series
	float E12[] = {3.3,3.9,4.7,5.6,6.8,8.2,10,12,15,18,22,27,33,39,
		47,56,68,82,91,100,110};

	return conform(C,E12,110);
}
float circuit::conform(int Z, float E[],int max){
	bool flag = false;
	int count = 0;

	while(true){
		if(Z<E[0]){
			return E[0];
		}else if(Z > max){
			return max;
		}else if(Z > E[count]){//not found value yet, keep going
			count ++;
		}else if(Z == E[count]){//value fits perfectly
			return E[count];
		}else if(E[count] == max){//end of run, 3k or larger
			return E[count];
		}else{//between two values
			if(count == 0){
				return E[0];
			}
			int a = E[count] - Z;
			int b = Z - E[count-1];
			if(a < b){
				return E[count];
			}else{
				return E[count-1];
			}
		}
	}
}