// RTx.h
// Author A Michel
// Date 20 08 15
// Lego2nano

#ifndef _RTX_h
#define _RTX_h

#if defined(ARDUINO) && ARDUINO >= 100
	#include "arduino.h"
#else
	#include "WProgram.h"
#endif

#define TRx_VERSION "0.0.1"

class RTx
{
 private:
	 unsigned long inputCount;
	 unsigned long outputCount;

 public:
	RTx();
	void reset();
	int sendData(int[], int);
    void sendString(String);
	String listen();
	bool echo;
};


#endif

