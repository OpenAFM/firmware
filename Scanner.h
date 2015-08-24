// Scanner.h

#ifndef _SCANNER_h
#define _SCANNER_h

#if defined(ARDUINO) && ARDUINO >= 100
	#include "arduino.h"
#else
	#include "WProgram.h"
#endif

#include "DACController.h"
#include "SignalSampler.h"
#include "RTx.h"


#define SCANNER_VERSION "0.0.1"

class Scanner
{

 private:
	 DACController controller;
	 SignalSampler sampler;
   RTx phone;
	 unsigned int linesScanned;
	 unsigned long startTime;
	 unsigned long endTime;
	 int lineLength;
	 int *pixels;
	 bool scanning;

 protected:

 public:
	 Scanner(const DACController&, const SignalSampler&, const RTx&, const int lineLength);
	 ~Scanner();
	 void reset();
	 int scanLine();
	 int start();
	 int stop();
};

#endif
