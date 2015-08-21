// Author A Michel
// Date 20 08 15
// Lego2Nano 

#include "Scanner.h"

Scanner::Scanner(const DACController &controller, const SignalSampler &sampler) : controller(controller), sampler(sampler) {
	this->startTime = 0;
	this->endTime = 0;
	Serial.print("Scanner(args)");
}

Scanner::~Scanner() {}

void Scanner::reset()
{
	controller.reset();
	sampler.reset();
	startTime = 0;
	endTime = 0;
	linesScanned = 0;
}


int Scanner::scanLine(int pixels[]) {
	unsigned int x = 0;
	for (x; x < controller.getLineSize(); x++) {
		pixels[x] = sampler.detectPixel();

		// don't move beyond eol
		if (x + 1 < controller.getLineSize())
			controller.increaseVoltage();
	}

	// TODO send data

	for (x; x < controller.getLineSize(); x++) {
		pixels[x] = sampler.detectPixel();

		// don't move beyond the BOL
		if (x - 1 > 0)
			controller.decreaseVoltage();
	}

	linesScanned++;
}

int Scanner::start(int lineLength) {

	// get start time
	startTime = millis();
	int* pixels = 0;
	pixels[lineLength * 2];

	scanLine(pixels);

}

int Scanner::stop() {

	// calculate lapsed time
	endTime = millis() - startTime;
}
