// 
// 
// 

#include "DACWriter.h"

const short CHANNEL_X = 0;
const short CHANNEL_Y = 1;
const short CHANNEL_Z = 2;

bool isInit = false;

void DACWriterClass::init(int stepSize, int dataPin, int clkPin, int loadPin, int ldacPin, bool rng, unsigned long serialRate)
{
	this->stepSize = stepSize;
	this->clkPin = clkPin;
	this->dataPin = dataPin;
	this->loadPin = loadPin;
	this->ldacPin = ldacPin;
	this->rng = rng;

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

	isInit = true;
}

int DACWriterClass::stepDn(int channel) {
	
	channel == CHANNEL_X ? this->currentX -= this->stepSize : 0;
	channel == CHANNEL_Y ? this->currentY -= this->stepSize : 0;
	channel == CHANNEL_Z ? this->currentZ -= this->stepSize : 0;
	return 1;
}

int DACWriterClass::stepUp(int channel) {
	channel == CHANNEL_X ? this->currentX += this->stepSize : 0;
	channel == CHANNEL_Y ? this->currentY += this->stepSize : 0;
	channel == CHANNEL_Z ? this->currentZ += this->stepSize : 0;
	return 1;
}

int DACWriterClass::reset(int channel) {
	channel == CHANNEL_X ? this->currentX = 0 : 0;
	channel == CHANNEL_Y ? this->currentY = 0 : 0;
	channel == CHANNEL_Z ? this->currentZ = 0 : 0;
	return 1;
}

int DACWriterClass::move(int channel) {
	int data = 0;

	//  Set DAC Channel LMB (left most significant bit)
	// 0 0 = A 0  X
	// 0 1 = B 1  Y
	// 1 0 = C 2  Z
	// 1 1 = D 3  -
	if (CHANNEL_X == channel) {
		sendLow(); 
		sendLow();
		// set data from 0 to 255
		data = this->currentX % 256;
	} else if (CHANNEL_Y == channel) {
		sendLow();
		sendHigh();
		data = this->currentY % 256;
	} else if (CHANNEL_Z == channel) {
		sendHigh();
		sendLow();
		data = this->currentZ % 256;
	}

	// 1x gain (0) or 2x (1)
	this->rng ? sendHigh() : sendLow();

	for (int i = 11; i >= 0; i--)
	{
		// shifting bits to the right.
		int k = data >> i;

		// true if k (LMS) is 1
		if (k & 1) {
			sendHigh();
		} else {
			sendLow();
		}
	}

	// load to output registers
	load();
}


int DACWriterClass::sendHigh() {
	digitalWrite(this->clkPin, HIGH);
	digitalWrite(this->dataPin, HIGH);
	digitalWrite(this->clkPin, LOW);
}

int DACWriterClass::sendLow() {
	digitalWrite(this->clkPin, HIGH);
	digitalWrite(this->dataPin, LOW);
	digitalWrite(this->clkPin, LOW);
}

int DACWriterClass::load() {
	digitalWrite(this->loadPin, LOW);
	digitalWrite(this->loadPin, HIGH);
}

DACWriterClass DACWriter;

