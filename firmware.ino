#include "DACWriter.h"
#inculde "DataRecv.h"
#include ? //DataSend

#define DACClkPin ?
#define DACDataPin ?
#define DataRecvPin ?

#define pixelCount ? //Pixels in one line
#define detectTimes ? //Samples of one pixel

DACWriterClass DACWriter;
DataRecvClass DataRecv;

<<<<<<< HEAD
=======
// the setup function runs once when you press reset or power the board
#include "DACWriter.h"

// Pin definitions
int SDI = 2;
int _SCK = 3;
int LOAD = 4;
int LDAC = 5;

int incomingByte;
int level;

// the setup routine runs once when you press reset:
>>>>>>> e68517c8af709fbc98cb35920df111da69ea000e
void setup() {
	pinMode(DACClkPin, OUTPUT);
	pinMode(DACDataPin, OUTPUT);
	pinMode(DataRecvPin, INPUT);

<<<<<<< HEAD
	/* Trigger the interrupt when pin 2 changes value*/
	/* See "https://www.arduino.cc/en/Reference/AttachInterrupt" */
	attachInterrupt(0, interrput, HIGH); 
}

void loop() {
	int pixelData[pixelCount];

	for (int i = 0; i < pixelCount; ++i) {
		/* Call DACWriter Function */
		
		/* Call DataRecv Function */
		pixelData[i] = DataRecv.detectPixel(DataRecvPin);
	}
	/* Send Data */

}

void interrupt() {
	delay(10);
=======
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
}

void writeHigh(){
	digitalWrite(_SCK, HIGH);
	digitalWrite(SDI, HIGH);
	digitalWrite(_SCK, LOW);
}

void writeLow(){
	digitalWrite(_SCK, HIGH);
	digitalWrite(SDI, LOW);
	digitalWrite(_SCK, LOW);
}

void loadDAC(){
	digitalWrite(LOAD, LOW);
	digitalWrite(LOAD, HIGH);
}

// channel max is 4095
void DACwrite(int input, int channel) {

	int k;

	// range from 0 to 255
	input = input % 256;

	// choose channel
	// 0 0 = A
	// 1 0 = B
	// 0 1 = C
	// 1 1 = D
	writeLow();
	writeLow();

	// 1x gain (0) or 2x (1)
	writeLow();

	// bit shifting to determine when to 
	// send 1 and when to send 0 in the command 
	// word.
	// sending most significant bit first
	// TODO negation ~ ??
	for (int c = 11; c >= 0; c--)
	{
		// 0110 - 6
		// -----
		// 0000 - 0 - 0
		// 0001 - 1 - 1
		// 0011 - 3 - ??
		// shifting bits to the right.
		k = input >> c;

		// true if k is 1
		if (k & 1){
			// this is a 1
			writeHigh();
		}
		else{
			// this is a 0
			writeLow();
		}

	}

	// load to output registers
	loadDAC();
}

// the loop routine runs over and over again forever:
void loop() {
	DACwrite(0, 0);
	delay(1000);
	DACwrite(64, 0);
	delay(1000);
	DACwrite(128, 0);
	delay(1000);
	DACwrite(196, 0);
	delay(1000);
	DACwrite(255, 0);
	delay(1000);
>>>>>>> e68517c8af709fbc98cb35920df111da69ea000e
}
