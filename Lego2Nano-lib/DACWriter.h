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

		 int stepSize;
		 bool rng;

		 int dataPin;
		 int clkPin;
		 int loadPin;
		 int ldacPin;

		 int currentX;
		 int currentY;
		 int currentZ;

	 public:
		void init(int, int, int, int, int, bool, unsigned long);
		void scanLine();
		int stepUp(int);
		int stepDn(int);
		int reset(int);
		int move(int);

	private:
		int sendHigh();
		int sendLow();
		int load();
	};

	extern DACWriterClass DACWriter;

	#endif

