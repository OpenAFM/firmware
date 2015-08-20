// 
// 
// 

#include "DACWriter.h"
#include "DataRecv.h"

const int CHANNEL_A = 0;
const int CHANNEL_B = 1;
const int CHANNEL_C = 2;
const int CHANNEL_D = 3;

void DACWriterClass::init(int stepSize, int lineSize, int dataPin, int clkPin, int loadPin, int ldacPin, bool rng, unsigned long serialRate)
{
	
	this->stepSize = stepSize;
	this->lineSize = lineSize;

	this->clkPin = clkPin;
	this->dataPin = dataPin;
	this->loadPin = loadPin;
	this->ldacPin = ldacPin;
	this->rng = rng;

	this->currentStep = 0;
	this->currentX = 0;
	this->currentY = 0;
	this->currentZ = 0;

	pinMode(this->dataPin, OUTPUT);
	pinMode(this->clkPin, OUTPUT);
	pinMode(this->loadPin, OUTPUT);
	pinMode(this->ldacPin, OUTPUT);

	Serial.begin(serialRate);

	digitalWrite(this->loadPin, HIGH);
	digitalWrite(this->ldacPin, LOW);

	// command is clocked on rising edge
	digitalWrite(this->clkPin, LOW);
}



int DACWriterClass::go(int channel, int value) {
	
	// Serial.print("channel: "); Serial.print(channel);
	// Serial.print(" value: "); Serial.println(value);
	//  Set DAC Channel LMB (left most significant bit)
	// 0 0 = A 0  X
	// 0 1 = B 1  Y
	// 1 0 = C 2  Z
	// 1 1 = D 3  -
	
	switch (channel)
	{
	case CHANNEL_A:
		sendLow();
		sendLow();
		break;
	case CHANNEL_B:
		sendLow();
		sendHigh();
		break;
	case CHANNEL_C:
		sendHigh();
		sendLow();
		break;
	case CHANNEL_D:
		sendHigh();
		sendHigh();
		break;
	}

	// 1x gain (0) or 2x (1)
	//rng ? sendHigh() : sendLow();
	sendLow();

	for (int i = 7; i >= 0; i--)
	{
		// shifting bits to the right.
		int k = value >> i;

		// true if k (LMS) is 1
		if (k & 1) {
			sendHigh();
		} else {
			sendLow();
		}
	}

	// load to output registers
	load();
	digitalWrite(dataPin, LOW);
}


int DACWriterClass::sendHigh() {
	digitalWrite(clkPin, HIGH);
	digitalWrite(dataPin, HIGH);
	digitalWrite(clkPin, LOW);
}

int DACWriterClass::sendLow() {
	digitalWrite(clkPin, HIGH);
	digitalWrite(dataPin, LOW);
	digitalWrite(clkPin, LOW);
}

int DACWriterClass::load() {
	digitalWrite(loadPin, LOW);
	digitalWrite(loadPin, HIGH);
}

int DACWriterClass::reset() {
	currentStep = 0;
	currentZ = 0;
	setCoordinates();
	go(CHANNEL_A, currentX);
	go(CHANNEL_B, currentY);
	return 1;
}

int DACWriterClass::nextLine() {
	int delta = (((currentStep / lineSize) + 1) * lineSize) - currentStep;
	currentStep += delta;
	setCoordinates();
}

int DACWriterClass::eol() {
	nextLine();
	currentStep--;
	setCoordinates();
}

int DACWriterClass::setCoordinates() {
	currentX = currentStep % lineSize;
	currentY = currentStep / lineSize;
}


int DACWriterClass::fwd() {

	// step fwd
	currentStep += stepSize;
	
	setCoordinates();
	
	go(CHANNEL_A, currentX);
	go(CHANNEL_B, currentY);

	return 0;
}

int DACWriterClass::bwd() {

	// step back
	currentStep -= stepSize;

	setCoordinates();

	go(CHANNEL_A, currentX);
	go(CHANNEL_B, currentY);

	return 0;
}
unsigned DACWriterClass::getLineSize() {
	return lineSize;
}

void DACWriterClass::print() {
	Serial.print("Step: "); Serial.print(currentStep); Serial.print(" LineSize: "); Serial.print(lineSize);
	Serial.print(" ["); Serial.print(currentX); Serial.print(", ");
	Serial.print(currentY); Serial.print("]");
	Serial.println();
	Serial.flush();
}

DACWriterClass DACWriter;

