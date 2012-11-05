//created: 23/10/12
//author: Simon Rankine
//updated: 30/10/12

#include "Arduino.h"
#include "motor.h"

class RS232{
   public:
     RS232();
     void listen();
     void establish(int pcBaud);
   private:
    boolean established;
    void call(boolean (motor::*member)(),motor,char c = ' ');
	void call(boolean(motor::*member)(int steps),motor go,char c = ' ');
    motor go;
};