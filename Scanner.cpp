// Author A Michel
// Date 20 08 15
// Lego2Nano 

#include "Scanner.h"

Scanner::Scanner(DACController controller, SignalSampler sampler) {
	this->sampler = sampler;
	this->controller = controller;
	this->startTime = 0;
	this->endTime = 0;
}

void Scanner::reset()
{
	controller.reset();
	sampler.reset();
	startTime = 0;
	endTime = 0;
	linesScanned = 0;
}


int Scanner::scanLine(int pixels[]) {

	for (unsigned int x = 0; x < controller.getLineSize(); x++) {
		pixels[x] = sampler.detectPixel();

		// don't move beyond eol
		if (x + 1 < controller.getLineSize())
			controller.increaseVoltage();
	}

	// TODO send data

	for (unsigned int x = 0; x < controller.getLineSize(); x++) {
		pixels[x] = sampler.detectPixel();

		// don't move beyond the BOL
		if (x - 1 > 0)
			controller.decreaseVoltage();
	}

	linesScanned++;
}

int Scanner::start() {
	// get start time
	startTime = millis();


}

int Scanner::stop() {

	// calculate lapsed time
	endTime = millis() - startTime;
}
