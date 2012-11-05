//created: 30/10/12
//author: Simon Rankine
//updated: 05/11/12

#include "Arduino.h"

class error{
public:
	static void set(int code);
	static void message(String mes);
	static void verbose(bool ver);
private:
	static bool speak;
	static String lookup(int code);
	static void printer(String message);
};