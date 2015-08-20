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
		 int lineSize;

		 bool rng;

		 int dataPin;
		 int clkPin;
		 int loadPin;
		 int ldacPin;

		 unsigned int currentStep;
		 int currentX;
		 int currentY;
		 int currentZ;

	 public:
		 void init(int, int, int, int, int, int, bool, unsigned long);
		int fwd();
		int bwd();
		int eol();
		unsigned getLineSize();

		int nextLine();
		int reset();
		void print();

	private:
		int sendHigh();
		int sendLow();
		int load();
		int go(int, int);
		int setCoordinates();
	};

	extern DACWriterClass DACWriter;

	#endif

