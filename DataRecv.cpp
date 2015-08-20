#include "DataRecv.h"

int DataRecvClass::init(short pin, short sampleSize) {
	this->pin = pin;
	this->sampleSize = sampleSize;
}

float DataRecvClass::detectPixel(RunningMedian samples)
{
	int temp = 0;

	for (int i = 0; i < sampleSize; i++) {
		temp = analogRead(pin);
		samples.add(temp);
	}
	double median = samples.getMedian();
	samples.clear();
	
	return median;
}

DataRecvClass DataRecv;

