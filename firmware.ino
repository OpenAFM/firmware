#include "DACWriter.h"
//#include "DataRecv.h"

/* Pin Definitions */
#define SDI 2;
#define SCK 3;
#define LOAD 4;
#define LDAC 5;
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

void interrupt() {
	delay(10);
}
