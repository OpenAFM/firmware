// Author A Michel
// Date 20 08 15
// Lego2Nano 

#include "Scanner.h"
#include "RTx.h"

Scanner::Scanner(const DACController &controller, const SignalSampler &sampler, const RTx &phone, const int lineLength) : pixels(new int[lineLength * 2]), controller(controller), sampler(sampler), phone(phone) {
	this->startTime = 0;
	this->endTime = 0;
	this->scanning = false;
	this->lineLength = lineLength;
	Serial.println("Scanner(args)");
}

Scanner::~Scanner() {
	delete[] pixels;
}

void Scanner::reset()
{
	controller.reset();
	sampler.reset();
	startTime = 0;
	endTime = 0;
	linesScanned = 0;
	scanning = false;
}


int Scanner::scanLine() {
	unsigned int x = 0;

	//trace
	for (int i = 0; i < controller.getLineSize(); i++) {
		pixels[x] = sampler.detectPixel();

		x++;
		if (i + 1 < controller.getLineSize())
			controller.increaseVoltage();
	}

	//retrace
	for (int i = 0; i < controller.getLineSize(); i++) {
		pixels[x] = sampler.detectPixel();

		x++;
		if(i + 1 < controller.getLineSize())
			controller.decreaseVoltage();
	}

	linesScanned++;

	return 0;
}

int Scanner::start() {

	// get start time
	startTime = millis();
	scanning = true;

	// interates over y-axis calling ctrl.nextLine() 
	for (int i = 0; i < lineLength; i++) {

		// scans one line (trace & re-trace)
		scanLine();

		// TODO send data
   phone.sendData(pixels, lineLength);

		// next line on y-axis
		unsigned int cl = controller.nextLine();

		if (scanning == false)
			break;
	}
}

int Scanner::stop() {

	// calculate lapsed time
	endTime = millis() - startTime;
	scanning = false;
}
