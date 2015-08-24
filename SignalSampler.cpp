// SingnalSampler.cpp
// Author Y_Vertex
// 
#include "SignalSampler.h"

SignalSampler::SignalSampler(void) {
	//TODO default values
	Serial.println("SignalSampler(void);");
}

SignalSampler::SignalSampler(int inputAPin, int inputBPin, int sampleSize) {
	this->init(inputAPin, inputBPin, sampleSize);
//analogReference(EXTERNAL);
Serial.println("SignalSampler(args);");
}

SignalSampler::~SignalSampler() {}

int SignalSampler::init(int inputAPin, int inputBPin, int sampleSize) {
	this->inputApin = inputAPin;
	this->inputBpin = inputBPin;
	this->sampleSize = sampleSize;
}

float SignalSampler::detectPixel()
{
	int aSignalValue = 0;
	int bSignalValue = 0;
  int cSignalValue = 0;
  
	for (int i = 0; i < sampleSize; i++) {
		// get the median of the samples
		// add the median
		aSignalValue = analogRead(inputApin);
		bSignalValue = analogRead(inputBpin);
    cSignalValue=aSignalValue+bSignalValue;    
	}
  //cSignalValue/=sampleSize;
 
	
	return aSignalValue;
}

int SignalSampler::reset() {
	return 0;
}


