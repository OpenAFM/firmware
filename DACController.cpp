#include "DACController.h"

const int CHANNEL_A = 0;
const int CHANNEL_B = 1;
const int CHANNEL_C = 2;
const int CHANNEL_D = 3;

#define ON HIGH
#define OFF LOW

DACController::DACController(int stepSize, int lineSize, int dataPin, int clockPin, int loadPin, int ldacPin, bool useRNG, unsigned long serialRate) {
	this->reset(stepSize, lineSize, dataPin, clockPin, loadPin, ldacPin, useRNG, serialRate);
}

void DACController::reset(int stepSize, int lineSize, int dataPin, int clockPin, int loadPin, int ldacPin, bool useRNG, unsigned long serialRate) {
	this->stepSize = stepSize;
	this->lineSize = lineSize;

	this->clkPin = clkPin;
	this->dataPin = dataPin;
	this->loadPin = loadPin;
	this->ldacPin = ldacPin;
	this->useRNG = useRNG;

	this->currentStep = 0;
	this->currentX = 0;
	this->currentY = 0;
	this->currentZ = 0;

	pinMode(this->dataPin, OUTPUT);
	pinMode(this->clkPin, OUTPUT);
	pinMode(this->loadPin, OUTPUT);
	pinMode(this->ldacPin, OUTPUT);

	Serial.begin(serialRate);

	digitalWrite(this->loadPin, ON);
	digitalWrite(this->ldacPin, OFF);

	// command is clocked on rising edge
	digitalWrite(this->clkPin, OFF);
}



int DACController::go(int channel, int value) {
	
	//  Set DAC Channel LMB (left most significant bit)
	// 0 0 = A  X
	// 0 1 = B  Y
	// 1 0 = C  Z
	// 1 1 = D  -
	
	switch (channel)
	{
		case CHANNEL_A:
			sendLowSignal();
			sendLowSignal();
			break;

		case CHANNEL_B:
			sendLowSignal();
			sendHighSignal();
			break;

		case CHANNEL_C:
			sendLowSignal();
			sendHighSignal();
			break;

		case CHANNEL_D:
			sendHighSignal();
			sendHighSignal();
			break;
	}

	// 1x gain (0) or 2x (1)
	useRNG ? sendHighSignal() : sendLowSignal();

	for (int i = 7; i >= 0; i--)
	{
		// shifting bits to the right.
		int k = value >> i;

		// true if k (LMS) is 1
		if (k & 1) {
			sendHighSignal();
		} else {
			sendLowSignal();
		}
	}

	// load to output registers
	loadDAC();
	digitalWrite(dataPin, OFF);
}


int DACController::sendHighSignal() {
	digitalWrite(clkPin, ON);
	digitalWrite(dataPin, ON);
	digitalWrite(clkPin, OFF);
}

int DACController::sendLowSignal() {
	digitalWrite(clkPin, ON);
	digitalWrite(dataPin, OFF);
	digitalWrite(clkPin, OFF);
}

int DACController::loadDAC() {
	digitalWrite(loadPin, OFF);
	digitalWrite(loadPin, ON);
}

int DACController::reset() {
	currentStep = 0;
	currentZ = 0;
	setCoordinates();
	go(CHANNEL_A, currentX);
	go(CHANNEL_B, currentY);
	return 1;
}

int DACController::nextLine() {
	int delta = (((currentStep / lineSize) + 1) * lineSize) - currentStep;
	currentStep += delta;
	setCoordinates();
}

int DACController::eol() {
	nextLine();
	currentStep--;
	setCoordinates();
}

int DACController::setCoordinates() {
	currentX = currentStep % lineSize;
	currentY = currentStep / lineSize;
}


int DACController::increaseVoltage() {

	// step fwd
	currentStep += stepSize;
	
	setCoordinates();
	
	go(CHANNEL_A, currentX);
	go(CHANNEL_B, currentY);

	return 0;
}

int DACController::decreaseVoltage() {

	// step back
	currentStep -= stepSize;

	setCoordinates();

	go(CHANNEL_A, currentX);
	go(CHANNEL_B, currentY);

	return 0;
}
int DACController::getLineSize() {
	return lineSize;
}

void DACController::print() {
	Serial.print("Step: "); Serial.print(currentStep); Serial.print(" LineSize: "); Serial.print(lineSize);
	Serial.print(" ["); Serial.print(currentX); Serial.print(", ");
	Serial.print(currentY); Serial.print("]");
	Serial.println();
	Serial.flush();
}


