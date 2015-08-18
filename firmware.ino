/*
 Name:		firmware.ino
 Created:	8/18/2015 3:49:25 AM
 Author:	User
*/

// the setup function runs once when you press reset or power the board
#include "DACWriter.h"

// Pin definitions
int SDI = 2;
int _SCK = 3;
int LOAD = 4;
int LDAC = 5;
int RNG = 0;


DACWriterClass writer;

// the setup routine runs once when you press reset:
void setup() {

	Serial.begin(115200);

	// initialize output pins
	pinMode(SDI, OUTPUT);
	pinMode(_SCK, OUTPUT);
	pinMode(LOAD, OUTPUT);
	pinMode(LDAC, OUTPUT);

	// activate DAC
	digitalWrite(LOAD, HIGH);
	// digitalWrite(LDAC, LOW);

	// command is clocked on rising edge
	digitalWrite(_SCK, LOW);

	// int stepSize, int dataPin, int clkPin, int loadPin, int ldacPin, bool rng, unsigned long serialRate
	writer.init(1, SDI, _SCK, LOAD, LDAC, RNG, 115200);
}


// the loop routine runs over and over again forever:
void loop() {
	writer.move(0);
	writer.stepUp(0);
	delay(30);
}
