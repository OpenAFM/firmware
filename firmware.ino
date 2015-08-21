
#include "Scanner.h"
#include "RTx.h"

/* Pin Definitions */
#define SDI 2
#define _SCK 3
#define LOAD 4
#define LDAC 5

#define STEPSIZE 1
#define RNG 0
#define BAUDRATE 9600
#define RECVPin A7

#define MAX 255

int pixelCount; // Parameter, Pixels in one line
int lineCount; // Parameter, The line number we will scan 
int detectTimes = 10; // Parameter, Times to detect one pixel
int pixelData[MAX] = { 0 };


Scanner ctrl();
RTx phone = RTx();

/* ================ */

void interrupt() {
	delay(10);
}

/* ================ */

void initialize() {Serial.print("got it;");
}

/* ================ */

void startWork() {
    ScanOneLine();
    //(pixelData);
}

void MoveToPreviousPixel(){
    //SCANNER.move(0);
    //SCANNER.stepDn(0);
}

void MoveToNextPixel(){
    //SCANNER.move(0);
    //SCANNER.stepUp(0);
}

void MoveToNextLine(){
    //SCANNER.move(1);
    //SCANNER.stepUp(1);
}


void ScanOneLine() {
	for (int i = 0; i < pixelCount; ++i) {
		/* Call DACWriter Function */
		//MoveToNextPixel();
		delay(25);
		/* Call DataRecv Function */
		//pixelData[i] = RECV.detectPixel(RECVPin, detectTimes); // calculate the avg for one pixel for one-direction scan
		delay(25);
	}
	for (int i = pixelCount - 1; i > -1; --i) {
		/* Call DACWriter Function */
		//MoveToPreviousPixel();
		delay(25);
		/* Call DataRecv Function */
		//pixelData[i] += RECV.detectPixel(RECVPin, detectTimes); // calculate the avg for one pixel for one-direction scan
		pixelData[i] /= 2;
		delay(25);
	}
}


/* ================ */

void setParameter() {
	pixelCount = ConvertStrToInt(Serial.readStringUntil(','));
	lineCount = ConvertStrToInt(Serial.readStringUntil(';'));
}

int ConvertStrToInt(String str) {
	return 0;
}

/* ================ */

void setup() {
	Serial.begin(BAUDRATE);
	//SCANNER.init(STEPSIZE, SDI, _SCK, LOAD, LDAC, RNG, BAUDRATE);
	pinMode(RECVPin, INPUT);
	/* Trigger the interrupt when pin 2 changes value*/
	/* See "https://www.arduino.cc/en/Reference/AttachInterrupt" */
	attachInterrupt(0, interrupt, HIGH);


	//ctrl = ControllerClass();
	//ctrl.init(1, 256, SDI, _SCK, LOAD, LDAC, false, 115200, 0, 5);
}

void loop() {
  Serial.println("send listen");
  String command=phone.listen();
  delay(200);
	if (command == "IN") initialize();
  else if (command == "SP") setParameter();
  else if (command == "SW") {
    for (int i = 0; i < lineCount; ++i) {
      startWork(); // work for one line
      //  MoveToNextLine();
      if (i == lineCount - 1)
      Serial.println("FINISH;");
      }
	}

	//ctrl.scanLine();
}
