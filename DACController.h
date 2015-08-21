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
		 int currentX;
		 int currentY;
		 int currentZ;

		 int sendHighSignal();
		 int sendLowSignal();
		 int loadDAC();
		 int go(int, int);
		 int setCoordinates();

	 public:
		 DACController();
		DACController(int, int, int, int, int, int, bool);
		~DACController();

		void reset(int, int, int, int, int, int, bool);
		int increaseVoltage();
		int decreaseVoltage();

		int eol();
		int getLineSize();
		int nextLine();
		int reset();

		void print();

	};

#endif

