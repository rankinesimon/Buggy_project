//created: 30/10/12
//author: Simon Rankine
//updated: 05/11/12

#include "error.h"

bool error::speak = false;

void error::set(int code){
	String message;
	if(!speak){
		message = "E";
		message += code;
		printer(message);
	}else{
		message = "E";
		message += code;
		message += ": ";
		message += lookup(code);
		printer(message);
	}
}

void error::verbose(bool ver){
	speak = ver;
}

void error::message(String mes){
	if(speak){
		printer(mes);
	}
}

void error::printer(String message){
	Serial1.print(message);
}

String error::lookup(int code){
	switch(code){
	case 1:
		//Example error
		return "This is a test error\n";
		break;
	case 2:
		//invalid command
		return "Un-known command recieved\n";
	case 3:
		//motor function failure
		return "Motor function failed";
	case 4:
		//expected a numeric charector
		return "Expected a numeric char";

	default:
		return "Un-known error code\n";
		break;
	}
	return "";//should never reach here...
}