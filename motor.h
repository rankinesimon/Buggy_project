#ifndef motor_h
#define motor_h

#include "Arduino.h"

class motor{
  public:
   motor();
   boolean forwards();
   boolean backwards();
   boolean clockwise();
   boolean anticlockwise();
   boolean run();
   boolean runBack();
   boolean stop();
  private: 
};

#endif