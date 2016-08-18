#include "SignalSampler.h"

SignalSampler::SignalSampler(void) {
	//TODO default values
	//Serial.println("SignalSampler(void);");
}

SignalSampler::SignalSampler(Adafruit_ADS1015& adc, int sampleSize):adc(adc) {
	this->init(sampleSize);
//this scales the input by the reference pin of the arduino
//Serial.println("SignalSampler(args);");
}

SignalSampler::~SignalSampler() {}

int SignalSampler::init(int sampleSize) {
	adc.begin();
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
		aSignalValue = adc.readADC_SingleEnded(0);
		bSignalValue = adc.readADC_SingleEnded(1);
   
		temp = aSignalValue+bSignalValue;
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



