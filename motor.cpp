//created: 23/10/12
//author: Simon Rankine
//updated: 30/10/12

#include "Arduino.h"
#include "motor.h"


motor::motor(){
  //this->comms = serial;
}

boolean motor::forwards(int step){
	analogWrite(2,127);
 return true; 
}

boolean motor::backwards(int step){
  return true;  
}

boolean motor::clockwise(){
  return true; 
}

boolean motor::anticlockwise(){
  return true;
}

boolean motor::run(){
  return true;
}

boolean motor::runBack(){
  return true;
}

boolean motor::stop(){
	analogWrite(2,0);
  return true;
}