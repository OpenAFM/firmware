#include "SignalSampler.h"

SignalSampler::SignalSampler(void) {
	//TODO default values
	Serial.println("SignalSampler(void);");
}

SignalSampler::SignalSampler(int inputAPin, int inputBPin, int sampleSize) {
	this->init(inputAPin, inputBPin, sampleSize);
//analogReference(EXTERNAL); //this scales the input by the reference pin of the arduino
Serial.println("SignalSampler(args);");
}

SignalSampler::~SignalSampler() {}

int SignalSampler::init(int inputAPin, int inputBPin, int sampleSize) {
	this->inputApin = inputAPin;
	this->inputBpin = inputBPin;
	this->sampleSize = sampleSize;
}

int SignalSampler::detectPixel()
{
	int aSignalValue = 0;
	int bSignalValue = 0;
	int *sumSignalValue;
	int j, temp;
	int median = 0;
	sumSignalValue = (int*)malloc(sizeof(int)*sampleSize);


	for (int i = 0; i < sampleSize; i++){
		// get aSignalValue, bSignalValue and add them
		aSignalValue = analogRead(inputApin);
		bSignalValue = analogRead(inputBpin);
		temp = aSignalValue + bSignalValue;
		// insert sorting the sum array
		j = i-1;
		while(j >= 0 && temp < sumSignalValue[j]){
			sumSignalValue[j+1] = sumSignalValue[j]; 
			j--;
		}
		sumSignalValue[j+1] = temp;
	}

	median = sumSignalValue[sampleSize/2];
	free(sumSignalValue);
	return median;
}

int SignalSampler::reset() {
	return 0;
}


