#include "Arduino.h"
#include "motor.h"

class RS232{
   public:
     RS232();
     void listen();
     void establish(int pcBaud);
   private:
    boolean established;
    void call(boolean (motor::*member)(),motor);
    motor go;
};