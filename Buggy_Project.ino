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
  //setup arduino pins
  pinMode(13, OUTPUT);//set up debug LED
  pinMode(0,INPUT);
  pinMode(1,OUTPUT);
  //open serial link
  link.establish(Baud);
  //init interrupt flag
  //interrupts::interrupted = false;
}

void loop(){
  link.listen();
}




