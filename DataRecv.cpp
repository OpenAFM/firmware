#include "DataRecv.h"

int DataRecvClass::detectPixel(int pin, int detectTimes)
{
	int temp = 0;
	int average = 0;
	for (int i = 0; i < detectTimes; ++i) {
		temp += analogRead(pin);
	}
	average = temp / detectTimes;
	return average;
}

DataRecvClass DataRecv;

