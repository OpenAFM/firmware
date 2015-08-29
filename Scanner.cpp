// Author A Michel
// Date 20 08 15
// Lego2Nano 2015

#include "Scanner.h"
#include "RTx.h"

// Constructor
Scanner::Scanner(const DACController &controller, const SignalSampler &sampler, const RTx &phone, const int lineLength) : pixels(new int[lineLength * 2]), controller(controller), sampler(sampler), phone(phone) {
	this->startTime = 0;
	this->endTime = 0;
	this->scanning = false;
	this->lineLength = lineLength;
}

// Destructor
Scanner::~Scanner() {
	delete[] pixels;
}

// reset. Stop the scanning process and resets all parameters.
void Scanner::reset()
{
	stop();
	// TODO implement proper interruption of process.
	delay(5000);

	controller.reset();
	sampler.reset();
	startTime = 0;
	endTime = 0;
	linesScanned = 0;
}

// Scans one line and stores data in the pixel array.
// Does a trace and retrace. The retrace data will be in backward order.
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

// Start the scanning process.
int Scanner::start() {

	// get start time
	startTime = millis();
	scanning = true;

	// interates over y-axis calling ctrl.nextLine() 
	for (int i = 0; i < lineLength; i++) {

		// scans one line (trace & re-trace)
		scanLine();

		scanning = phone.sendData(pixels, 2 * lineLength);

		// next line on y-axis
		unsigned int cl = controller.nextLine();

		if (scanning == false) {
			break;
		}
	}
	stop();
}

// stop the scanning process and resets the parameters.
int Scanner::stop() {

	// calculate lapsed time
	endTime = millis() - startTime;
	scanning = false;
}

// return the lapsed time
unsigned long Scanner::getLapsedTime() {
	return endTime;
}


extern String const PARAM_LINE_LENGTH = "LINELENGTH";

void Scanner::setParam(String param, String value) {
	stop();

	if (PARAM_LINE_LENGTH == param) {
		lineLength = atoi(value.c_str());
	}

}

void Scanner::invertChannels() {
	controller.invert();
}