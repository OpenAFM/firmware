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
	 int readPin;
	 int writePin;
	 boolean isSerial;
	 unsigned long inputCount;
	 unsigned long outputCount;

 public:
	RTx(int, int, bool);
	void reset();
	int send(int[]);
	int read(int[]);
	String read();
};


#endif

