
#include "Scanner.h"
#include "RTx.h"

/* Pin Definitions */
#define _SDI 2
#define _SCK 3
#define LOAD 4
#define LDAC 5

#define STEPSIZE 1
#define RNG 0
#define BAUDRATE 9600
#define RECVPin A7
#define LINE_LENGTH 255

#define SAMPLER_PIN_A 7
#define SAMPLER_PIN_B 8

int pixelCount=10;
int lineCount=10;
int pixelData[200];

RTx phone = RTx();
DACController ctrl = DACController(STEPSIZE, LINE_LENGTH, _SDI, _SCK, LOAD, LDAC, RNG);
SignalSampler sampler = SignalSampler(SAMPLER_PIN_A, SAMPLER_PIN_B, 5);
Scanner scanner = Scanner(ctrl, sampler);

/* ================ */
void initialize() {Serial.print("got it;");
}


void scanLine() {
	  //scanner.scanLine();
		
}
/* ================ */

void setParameter() {
	//pixelCount = ConvertStrToInt(Serial.readStringUntil(','));
	//lineCount = ConvertStrToInt(Serial.readStringUntil(';'));
}

int ConvertStrToInt(String str) {
	return 0;
}

/* ================ */

void setup() {
	Serial.begin(BAUDRATE);
}

void loop() {
  phone.sendString("send listen");
  String command=phone.listen();
  delay(200);

	//ctrl.scanLine();
}
