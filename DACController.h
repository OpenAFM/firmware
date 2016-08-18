#include <Wire.h>

// DACWriter.h
// Author A Michel
// Date 25/08/15
// lego2nano 2015

#ifndef _DACCONTROLLER_h
#define _DACCONTROLLER_h

#if defined(ARDUINO) && ARDUINO >= 100
#include "arduino.h"
#else
#include "WProgram.h"
#endif

#define DAC_TLC_5620_CONTROLLER "0.0.1"

// Defines a digital to analog controller for TLC5620CN
class DACController {

private:
	// Number of increments per pixel
	int stepSize;
	// number of pixels in line
	int lineSize;
	// amplification
	bool useRNG;

	// data pin number
	int dataPin;
	// clock pin number
	int clockPin;
	// load pin number
	int loadPin;
	// ldac pin number
	int ldacPin;

	// the current pixel position in the image.
	// each pixel has a unique x,yi position.
	unsigned int currentStep;

	// sends an on bit to the chip
	int setBitOn();
	// sends an off bit to the chip
	int setBitOff();

	// load DAC
	int loadDAC();

	// send data to dac.
	int go(int, int);

	// set the x & y coordinates
	int setCoordinates();

	// current x position in the matrix
	int currentX;
	// current y position in the matrix
	int currentY;
	// not used; intended to store height information.
	int currentZ;

	// scan 90-degree angle
	bool invertChannels;

public:
	// constructor
	DACController(int, int, int, int, int, int, bool);
	
	// destructor
	~DACController();
	
	// reset parameters
	unsigned int reset(int, int, int, int, int, int, bool);

	// increase voltage by one step (8 bit steps given by chip)
	unsigned int increaseVoltage();
	// decrease voltage by one step
	unsigned int decreaseVoltage();

	// go to end of current line
	// returns current point in matrix.
	unsigned int eol();

	// return line size.
	int getLineSize();

	// move to the first pixel of the new line.
	unsigned int nextLine();

	// reset to x & y coordinates to 0,0
	unsigned int reset();

	// return current voltage for selected channel (A - x, B - y, C - z).
	int getVoltage(int);

	// invert channel A(X) & B(Y)
	void invert();
};

#endif

