//created: 23/10/12
//author: Simon Rankine
//updated: 23/10/12

#include "Arduino.h"
#include "rs232.h"

class interrupts{
public:
	static bool interrupted;
	static void rs232Interrupt();
};