#include "Arduino.h"


class motor{
  public:
   motor();
   boolean forwards(int steps = 1);
   boolean backwards(int steps = 1);
   boolean clockwise();
   boolean anticlockwise();
   boolean run();
   boolean runBack();
   boolean stop();
  private:
	//RS232 comms;
};