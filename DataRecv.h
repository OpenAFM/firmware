// DataRecv.h

#ifndef _DATARECV_h
#define _DATARECV_h

#if defined(ARDUINO) && ARDUINO >= 100
	#include "arduino.h"
#else
	#include "WProgram.h"
#endif

#include <RunningMedian.h>

class DataRecvClass
{
	protected:

	private: 
	short pin;
	int sampleSize;

	public:
		int init(short, short);
		float detectPixel(RunningMedian samples);
};

extern DataRecvClass DataRecv;

#endif

