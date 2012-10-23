//created: 21/10/12
//author: Simon Rankine
//updated: 23/10/12

#include "motor.h"
#define pcBaud 9600//baud rate for PC RS232 communications

class RS232{
   public:
     RS232();
     void listen();
     void establish();
   private:
    boolean established;
    void call(boolean (motor::*member)(),motor);
    motor go;
};

RS232::RS232(){

}

void RS232::establish(){
    //attempts to establish RS232 communication
  established=false;
  Serial.begin(pcBaud);//establish serial link using defined baud rate
  char c;
  while(c != '&'){
    //wait for dummy serial establishment charector
    c = Serial.read();
  }
  established = true;
  //trun on debug LED
  digitalWrite(13,HIGH);
  //send acknoledgement of connection
  Serial.println('&');
}

void RS232::listen(){
      if(Serial.available() > 0){//check for new serial data
        char c = Serial.read();//read in serial data
        //decide what to do
        switch(c){
          case 'F':
            call(&motor::forwards,go);
            break;
          case 'B':
            call(&motor::backwards,go);
            break;
          case 'A':
            call(&motor::anticlockwise,go);
             break;
          case 'C':
             call(&motor::clockwise,go);
             break;
          case 'S':
             call(&motor::stop,go);
             break;
           case '&':
             Serial.println("&");
             break;
           case 'f':
             call(&motor::run,go);
             break;
           case 'b':
             call(&motor::runBack,go);
             break;
           case '-1':
             break;
           default:
             Serial.println("E");
             break;
        }
      }
}

void RS232::call(boolean(motor::*member)(),motor go){
  Serial.println("#");
  if((go.*member)()){
    Serial.println("*");
  }else{
     Serial.println("E"); 
  }
}

//define a serial port
boolean led = false;
RS232 link;

void setup(){
  //setup arduino pins
  pinMode(13, OUTPUT);//set up debug LED
  //open serial link
  link.establish();
}

void loop(){
  link.listen();
}


