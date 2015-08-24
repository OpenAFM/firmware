
#include "Scanner.h"
#include "RTx.h"

/* Pin Definitions */
#define _SDI 2
#define _SCK 3
#define LOAD 4
#define LDAC 5
#define RNG false
#define BAUDRATE 9600

#define STEPSIZE 1
#define LINE_LENGTH  256


/* Sampler PIN Definitions */
#define SAMPLER_PIN_A A7
#define SAMPLER_PIN_B A8
#define SAMPLE_SIZE 5


/* ================ */

void setup() {
	Serial.begin(115200);
	Serial.print("Setup");
	Serial.println("Setup done.");
}

extern const short START_CMD = 0;
extern const short STOP_CMD = 1;




DACController dacController = DACController(STEPSIZE, LINE_LENGTH, _SDI, _SCK, LOAD, LDAC, RNG);
SignalSampler sampler = SignalSampler(SAMPLER_PIN_A, SAMPLER_PIN_B, 5);
Scanner scanner = Scanner(dacController, sampler, LINE_LENGTH);

void loop() {

	/*
	int cmd = rtx.getCommand();

	switch (cmd) {
		case START_CMD:
			break;
		case STOP_CMD:
			break;
		default:
			break;
	}
	*/
	/* Scanner definition */
	
	

	scanner.reset();
	delay(1000);
	while (true) {
		scanner.start();
		delay(8000);
		scanner.reset();
		delay(2000);
	}
}
