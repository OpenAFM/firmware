// DataRecv.h

#ifndef _DATARECV_h
#define _DATARECV_h

#if defined(ARDUINO) && ARDUINO >= 100
	#include "arduino.h"
#else
	#include "WProgram.h"
#endif

class DataRecvClass
{
	protected:

	public:
		int detectPixel(int);
};

extern DataRecvClass DataRecv;

#endif

