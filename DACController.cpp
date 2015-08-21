#include "DACController.h"

#define ON HIGH
#define OFF LOW

const int CHANNEL_A = 0;
const int CHANNEL_B = 1;
const int CHANNEL_C = 2;
const int CHANNEL_D = 3;

DACController::DACController(void) {
	//TODO default values
	Serial.println("DACController(void)");
}

DACController::DACController(int stepSize, int lineLength, int dataPin, int clockPin, int loadPin, int ldacPin, bool useRNG) {
	this->stepSize = stepSize;
	this->lineSize = lineLength;

	this->clockPin = clockPin;
	this->dataPin = dataPin;
	this->loadPin = loadPin;
	this->ldacPin = ldacPin;
	this->useRNG = useRNG;

	this->currentStep = 0;
	this->currentX = 0;
	this->currentY = 0;
	this->currentZ = 0;

	pinMode(this->dataPin, OUTPUT);
	pinMode(this->clockPin, OUTPUT);
	pinMode(this->loadPin, OUTPUT);
	pinMode(this->ldacPin, OUTPUT);

	digitalWrite(this->loadPin, HIGH);
	digitalWrite(this->ldacPin, LOW);

	// command is clocked on rising edge
	digitalWrite(this->clockPin, LOW);

	Serial.println("DACController(args)");
}

DACController::~DACController() {}

void DACController::reset(int stepSize, int lineSize, int dataPin, int clockPin, int loadPin, int ldacPin, bool useRNG) {
	this->stepSize = stepSize;
	this->lineSize = lineSize;

	this->clockPin = clockPin;
	this->dataPin = dataPin;
	this->loadPin = loadPin;
	this->ldacPin = ldacPin;
	this->useRNG = useRNG;

	this->currentStep = 0;
	this->currentX = 0;
	this->currentY = 0;
	this->currentZ = 0;

	pinMode(this->dataPin, OUTPUT);
	pinMode(this->clockPin, OUTPUT);
	pinMode(this->loadPin, OUTPUT);
	pinMode(this->ldacPin, OUTPUT);

	digitalWrite(this->loadPin, HIGH);
	digitalWrite(this->ldacPin, LOW);

	// command is clocked on rising edge
	digitalWrite(this->clockPin, LOW);
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
}


int DACController::sendHighSignal() {
	digitalWrite(clockPin, HIGH);
	digitalWrite(dataPin, HIGH);
	digitalWrite(clockPin, LOW);
}

int DACController::sendLowSignal() {
	digitalWrite(clockPin, HIGH);
	digitalWrite(dataPin, LOW);
	digitalWrite(clockPin, LOW);
}

int DACController::loadDAC() {
	digitalWrite(loadPin, LOW);
	digitalWrite(loadPin, HIGH);

	digitalWrite(dataPin, LOW);
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


