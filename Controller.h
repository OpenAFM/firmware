// Controller.h

#ifndef _CONTROLLER_h
#define _CONTROLLER_h

#if defined(ARDUINO) && ARDUINO >= 100
	#include "arduino.h"
#else
	#include "WProgram.h"
#endif

#include "DACWriter.h"
#include "DataRecv.h"

class ControllerClass
{

 private:
	 DACWriterClass writer;
	 DataRecvClass sampler;
	 unsigned int totalSteps;
	 int lineSize;
	 short sampleSize;

 protected:

 public:
	 void init(int, int, int, int, int, int, bool, unsigned long, int, int);
	 int scanLine();

};

//extern ControllerClass Controller;

#endif

