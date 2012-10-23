//created: 21/10/12
//author: Simon Rankine
//updated: 23/10/12

#include "rs232.h"
#define Baud 9600//baud rate for PC RS232 communications

//define a serial port
boolean led = false;
RS232 link;

void setup(){
  //setup arduino pins
  pinMode(13, OUTPUT);//set up debug LED
  //open serial link
  link.establish(Baud);
}

void loop(){
  link.listen();
}


