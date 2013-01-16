//created: 21/10/12
//author: Simon Rankine
//updated: 23/10/12

//#include "interrupt.h"
#include "rs232.h"
#include "Arduino.h"
#define Baud 9600//baud rate for PC RS232 communications

//define a serial port
boolean led = false;
RS232 link;

void setup(){
  initPins();
  //open serial link
  link.establish(Baud);
  //init interrupt flag
  //interrupts::interrupted = false;
  analogReference(EXTERNAL);
}

void initPins(){
	//setup arduino pins
	pinMode(13, OUTPUT);//set up debug LED
	pinMode(0,INPUT);
	pinMode(1,OUTPUT);
	pinMode(23,OUTPUT);
	pinMode(25,OUTPUT);
	pinMode(27,OUTPUT);
	pinMode(29,OUTPUT);
	pinMode(31,OUTPUT);
	pinMode(33,OUTPUT);
	pinMode(34,OUTPUT);
	pinMode(35,OUTPUT);
	pinMode(37,OUTPUT);
	pinMode(39,OUTPUT);
	pinMode(41,OUTPUT);
	pinMode(A0,INPUT);
	pinMode(A1,INPUT);
	pinMode(A2,INPUT);
	pinMode(A3,INPUT);
	pinMode(A4,INPUT);
}

void loop(){
  link.listen();
}




