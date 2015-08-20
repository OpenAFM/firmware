// 
// 
// 

#include "RTx.h"

RTx::RTx(int readPin, int writePin, boolean isSerial)
{
	this->inputCount = 0;
	this->outputCount = 0;
	this->writePin = writePin;
	this->readPin = readPin;
	this->isSerial = isSerial;
	pinMode(this->writePin, OUTPUT);
	pinMode(this->readPin, INPUT);
}

void RTx::reset() {
	this->inputCount = 0;
	this->outputCount = 0;
}

int RTx::send(int[]){
	// TODO: write data to writePin if isSerial == false
	// else write data to serial
}

int RTx::read(int[]){
	// TODO: read data from readPin until end of input array
	// (must know the size of data) or read from serial if isSerial == true (?)
}

String RTx::read() {
	// TODO: read a string from serial
}

