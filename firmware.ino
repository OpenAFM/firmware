#include "stdafx.h"
#include "Scanner.h"
#include "RTx.h"

/* Pin Definitions */

//Pins for DAC (tlc5620)
#define LDAC 10 //ldac HIGH to stop DAC output while clocking. 
#define RNG 0   //HIGH doubles the output voltage

//Imaging parameters
#define STEPSIZE 1      //Stepsize of DAC(linelength/stepsize=number of pixel per line)
#define LINE_LENGTH 10 //Voltage range over (MAX256). Defines size of image in real space
#define SAMPLE_SIZE 5   //Number of samples taken at each pixel. Median is taken as true value

//Communication parameters
#define BAUDRATE 115200   //Serial interfaces communication speed (bps)


/* Setup */
Adafruit_ADS1015 adc;
RTx* phone=new RTx();
PiezoDACController* ctrl=new PiezoDACController(STEPSIZE, LINE_LENGTH, LDAC, RNG);
SignalSampler* sampler=new SignalSampler(adc, SAMPLE_SIZE);
Scanner* scanner=new Scanner(*ctrl, *sampler, *phone, LINE_LENGTH);

//This function runs once, when the arduino starts
void setup() {
	Serial.begin(BAUDRATE);
	ADDAC::Setup(LDAC);
}

extern String const PARAM_LINE_LENGTH;

//This function keeps looping
void loop() {

	String cmd = phone->listen();
  
	delay(1);
 
	if (cmd == "GO")
	{
		scanner->start();
	}
	else if (cmd == "SETUP")
	{
		delete ctrl;
		delete sampler;
		delete scanner;
      
		int CUSTOM_STEPSIZE = Serial.parseInt();
		int CUSTOM_LINE_LENGTH = Serial.parseInt();
		int CUSTOM_SAMPLE_SIZE = Serial.parseInt();
     
		ctrl = new PiezoDACController(CUSTOM_STEPSIZE, CUSTOM_LINE_LENGTH, LDAC, RNG);
		sampler = new SignalSampler(adc, CUSTOM_SAMPLE_SIZE);
		scanner = new Scanner(*ctrl, *sampler, *phone, CUSTOM_LINE_LENGTH);     
    
	}
	else if (cmd == "STREAM")
	{
		scanner->stream();
	}
	else if (cmd == "ERROR")
	{

	}
	else if (cmd == "PING")
	{
		phone->sendString("PONG\r\n");
	}
  
}
