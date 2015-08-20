#include "DACWriter.h"
//#include "DataRecv.h"
#include "Controller.h"

/* Pin Definitions */
const int SDI = 2;
const int CLK = 3;
const int LOAD = 4;
const int LDAC = 5;
const int RNG= 0;

//#define DataRecvPin ?

//#define pixelCount ? // Pixels in one line
//#define detectTimes ? // Samples of one pixel


//DACWriterClass writer;
//DataRecvClass DataRecv;
ControllerClass ctrl;

void setup() {
      //  pinMode(DataRecvPin, INPUT);
        
        /* Trigger the interrupt when pin 2 changes value*/
        /* See "https://www.arduino.cc/en/Reference/AttachInterrupt" */
       // attachInterrupt(0, interrput, HIGH); 

	// int stepSize, int dataPin, int clkPin, int loadPin, int ldacPin, bool rng, unsigned long serialRate
	//writer.init(1, 256, SDI, CLK, LOAD, LDAC, false, 115200);
		//1, SDI, _SCK, LOAD, LDAC, RNG, 115200);
	//byte lineSize = 255;
	ctrl = ControllerClass();
	ctrl.init(1, 256, SDI, CLK, LOAD, LDAC, false, 115200, 0, 5);
}

void loop() {
       /* int pixelData[pixelCount];
    
        for (int i = 0; i < pixelCount; ++i) {
            	/* Call DACWriter Function */
        	
        	/* Call DataRecv Function */
        //        pixelData[i] = DataRecv.detectPixel(DataRecvPin);
        //}
	
	//writer.reset();
	/*for (unsigned int i = 0; i < 25500; i++) {
		writer.fwd();
	}*/

	/*for (long i = 0; i < 255; i++) {
		writer.bwd();
		delay(20);
	}*/

	

	//writer.reset();
	//delay(5000);

	//writer.eol();
	//delay(5000);

	ctrl.scanLine();



        /* Send Data */


}

void interrupt() {
	delay(10);
}
