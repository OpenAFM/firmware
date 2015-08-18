#include "DACWriter.h"
<<<<<<< HEAD
#include "DataRecv.h"
=======
//#include "DataRecv.h"
>>>>>>> 298263cbfb8e117f515ae14f4bc1a5731d82ba98

/* Pin Definitions */
#define SDI 2;
#define SCK 3;
#define LOAD 4;
#define LDAC 5;
<<<<<<< HEAD
#define RECVPin A7

DACWriterClass WRITER;
DataRecvClass RECV;

int pixelCount = 8; // Parameter
int detectTimes = 10; // Parameter
int pixelData[pixelCount] = {0};

=======
#define RNG 0;
//#define DataRecvPin ?

//#define pixelCount ? // Pixels in one line
//#define detectTimes ? // Samples of one pixel


DACWriterClass writer;
//DataRecvClass DataRecv;


void setup() {
      //  pinMode(DataRecvPin, INPUT);
        
        /* Trigger the interrupt when pin 2 changes value*/
        /* See "https://www.arduino.cc/en/Reference/AttachInterrupt" */
       // attachInterrupt(0, interrput, HIGH); 

	// int stepSize, int dataPin, int clkPin, int loadPin, int ldacPin, bool rng, unsigned long serialRate
	//writer.init(1, SDA, SCK, LOAD, LDAC, true, 115200);
		//1, SDI, _SCK, LOAD, LDAC, RNG, 115200);
}

void loop() {
       /* int pixelData[pixelCount];
    
        for (int i = 0; i < pixelCount; ++i) {
            	/* Call DACWriter Function */
        	
        	/* Call DataRecv Function */
        //        pixelData[i] = DataRecv.detectPixel(DataRecvPin);
        //}
		
	writer.move(0);
	writer.stepUp(0);
	delay(30);

        /* Send Data */

}
>>>>>>> 298263cbfb8e117f515ae14f4bc1a5731d82ba98

void interrupt() {
    delay(10);
}
<<<<<<< HEAD

void dataSend(int* array) {
    for (int i = 0; i < pixelCount; ++i) {
        Serial.print(array[i]);
        if (i == pixelCount - 1)
            Serial.print(';');
        else
            Serial.print(',');
    }
}

void initialize() {
}

void startWork() {
    for (int i = 0; i < pixelCount; ++i) {
        /* Call DACWriter Function */
    	
    	/* Call DataRecv Function */
        pixelData[i] = RECV.detectPixel(RECVPin, detectTimes);
    }
    /* Send Data */
    dataSend(pixelData);
}

void setParameter() {
}


void setup() {
    dataSend(pixelData);
    Serial.begin(9600);
    pinMode(RECVPin, INPUT);
    /* Trigger the interrupt when pin 2 changes value*/
    /* See "https://www.arduino.cc/en/Reference/AttachInterrupt" */
    attachInterrupt(0, interrupt, HIGH); 
}

void loop() {
    if (Serial.available()) {
        String command = Serial.readStringUntil(';');
        if (command == "IN") initialize();
        else if (command == "SW") startWork();
        else if (command == "SP") setParameter();
        delay(1);
    }
}
=======
>>>>>>> 298263cbfb8e117f515ae14f4bc1a5731d82ba98
