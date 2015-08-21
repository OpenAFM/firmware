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

#define SCANNER_VERSION "0.0.1"

class Scanner
{

 private:
	 DACController controller;
	 SignalSampler sampler;
	 unsigned int linesScanned;
	 unsigned long startTime;
	 unsigned long endTime;

 protected:

 public:
	 Scanner(const DACController&, const SignalSampler&);
	 ~Scanner();
	 void reset();
	 int scanLine(int[]);
	 int start(int);
	 int stop();
};

#endif
