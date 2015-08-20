#include "SignalSampler.h"


SignalSampler::SignalSampler(int inputAPin, int inputBPin, int sampleSize) {
	this->init(inputAPin, inputBPin, sampleSize);
}

int SignalSampler::init(int inputAPin, int inputBPin, int sampleSize) {
	this->inputApin = inputAPin;
	this->inputBpin = inputBPin;
	this->sampleSize = sampleSize;
}

float SignalSampler::detectPixel()
{
	int aSignalValue = 0;
	int bSignalValue = 0;

	for (int i = 0; i < sampleSize; i++) {
		aSignalValue = analogRead(inputApin);
		bSignalValue = analogRead(inputBpin);
	}
	
	return 0;
}

int SignalSampler::reset() {
	return 0;
}


