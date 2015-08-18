#include "DACWriter.h"
#include "DataRecv.h"

/* Pin Definitions */
#define SDI 2
#define SCK 3
#define LOAD 4
#define LDAC 5
#define RECVPin A7

#define pixelCount 8 // Pixels in one line

int detectTimes = 10; // Parameter, Times to detect one pixel
int pixelData[pixelCount] = {0};

DACWriterClass WRITER;
DataRecvClass RECV;

void interrupt() {
    delay(10);
}

void dataSend(int* array) {
    for (int i = 0; i < pixelCount; ++i) {
        Serial.print(array[i]);
        if (i == pixelCount - 1)
            Serial.print(';'); // Send ';' when all numbers are sent
        else
            Serial.print(','); // Send ',' when a number is sent
    }
}

void initialize() {
}

void startWork() {
    for (int i = 0; i < pixelCount; ++i) {
        /* Call DACWriter Function */
	WRITER.move(0);
	WRITER.stepUp(0);
	delay(25);
    	/* Call DataRecv Function */
        pixelData[i] = RECV.detectPixel(RECVPin, detectTimes);
        delay(25);
    }
    /* Send Data */
    dataSend(pixelData);
}

void setParameter() {
}

void setup() {
    Serial.begin(9600);
    pinMode(RECVPin, INPUT);
    /* Trigger the interrupt when pin 2 changes value*/
    /* See "https://www.arduino.cc/en/Reference/AttachInterrupt" */
    attachInterrupt(0, interrupt, HIGH); 
}

void loop() {
    if (Serial.available()) {
        String command = Serial.readStringUntil(';'); // ';' Stands for the end of a command
        if (command == "IN") initialize();
        else if (command == "SW") startWork();
        else if (command == "SP") setParameter();
        delay(1);
    }
}
