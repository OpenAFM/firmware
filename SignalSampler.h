#include "stdafx.h"

// DataRecv.h
// Author Y_Vertex

#ifndef _SIGNALSAMPLER_h
#define _SIGNALSAMPLER_h

#if defined(ARDUINO) && ARDUINO >= 100
	#include "arduino.h"
#else
	#include "WProgram.h"
#endif


#define SIGNAL_SAMPLER "0.0.1"

class SignalSampler
{
	protected:

	private: 
		Adafruit_ADS1015 adc;
		int sampleSize;

	public:
		SignalSampler(void);
		SignalSampler(Adafruit_ADS1015&, int);
		~SignalSampler();
		int init(int);
		int detectPixel();
		int reset();
};

#endif

