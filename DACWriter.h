// DACWriter.h

#ifndef _DACWRITER_h
#define _DACWRITER_h

#if defined(ARDUINO) && ARDUINO >= 100
	#include "arduino.h"
#else
	#include "WProgram.h"
#endif

class DACWriterClass
{
 protected:


 public:
	void init();
};

extern DACWriterClass DACWriter;

#endif

