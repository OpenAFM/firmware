	// DACWriter.h

	#ifndef _DACCONTROLLER_h
	#define _DACCONTROLLER_h

	#if defined(ARDUINO) && ARDUINO >= 100
		#include "arduino.h"
	#else
		#include "WProgram.h"
	#endif

	#define DAC_TLC_5620_CONTROLLER "0.0.1"
	
class DACController {

	 private:
		 int stepSize;
		 int lineSize;

		 bool useRNG;

		 int dataPin;
		 int clockPin;
		 int loadPin;
		 int ldacPin;

		 unsigned int currentStep;

		 int setBitOn();
		 int setBitOff();
		 int loadDAC();
		 int go(int, int);
		 int setCoordinates();

	 public:
		DACController();
		DACController(int, int, int, int, int, int, bool);
		~DACController();

		int currentX;
		int currentY;
		int currentZ;

		unsigned int reset(int, int, int, int, int, int, bool);
		unsigned int increaseVoltage();
		unsigned int decreaseVoltage();

		unsigned int eol();
		int getLineSize();
		unsigned int nextLine();
		unsigned int reset();
		int getVoltage(int);

		void print();

	};

#endif

