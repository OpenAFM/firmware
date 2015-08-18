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

void setup() {
	pinMode(DACClkPin, OUTPUT);
	pinMode(DACDataPin, OUTPUT);
	pinMode(DataRecvPin, INPUT);

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
}
