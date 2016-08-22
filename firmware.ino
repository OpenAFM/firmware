#include "stdafx.h"
#include "Scanner.h"
#include "RTx.h"
#include "DAC_AD5696.h"

/* Pin Definitions */

//Pins for DAC (tlc5620)
#define LDAC 10 //ldac HIGH to stop DAC output while clocking. 
#define RNG 0   //HIGH doubles the output voltage

//Imaging parameters
#define STEPSIZE 1      //Stepsize of DAC(linelength/stepsize=number of pixel per line)
#define LINE_LENGTH 10 //Voltage range over (MAX256). Defines size of image in real space
#define SAMPLE_SIZE 5   //Number of samples taken at each pixel. Median is taken as true value

//Communication parameters
#define BAUDRATE 9600   //Serial interfaces communication speed (bps)



int splitString(String str, char delimiter, String *out)
{
    int len = str.length();
    int pos = 0;
    int pos2 = 0;
    int count = 0;

    while(true)
    {
      // find the next delimiter
      pos = str.indexOf(delimiter, pos);

      // ended?
      if (pos == -1) break;

      // extract the string
      String part = str.substring(pos, pos2);
    }
}



/* Setup */
Adafruit_ADS1015 adc;
RTx* phone = new RTx();
PiezoDACController* ctrl = new PiezoDACController(STEPSIZE, LINE_LENGTH, LDAC, RNG);
SignalSampler* sampler = new SignalSampler(adc, SAMPLE_SIZE);
Scanner* scanner = new Scanner(*ctrl, *sampler, *phone, LINE_LENGTH);
unsigned char zchar = (unsigned char)0;
DAC_AD5696* dac = new DAC_AD5696();

//This function runs once, when the arduino starts
void setup() {
	Serial.begin(BAUDRATE);
	ADDAC::Setup(LDAC);
  dac->Init(0,0);
}

extern String const PARAM_LINE_LENGTH;

//This function keeps looping
void loop() {

	String cmd = phone->listen();

/*
  Serial.println("The message was " + cmd + " ok?");
  if (cmd == "PING")
  {
    Serial.println("PONG");
  }
*/


	//delay(1);
  int idx;

   /*
    * Get command over serial
    * PDAC::SET <x> <y>  
    *   Set the channel <x> voltage of the piezo DAC to <y>
    */
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
		Serial.println("PONG");
	}
	else
  {
    //////////////////////////////////////////////////////
    // SET DAC VOLTAGE (FOR DEBUG)
    //////////////////////////////////////////////////////
    
    if (idx = cmd.indexOf("PDAC::SET") == 0)
    {
      //Serial.println("DACC!!!");
      bool ok = false;
      String channelPart;
      String valuePart;
      int channel;
      float value;
      while(1)
      {
        String *parts;
        //int num = splitString(cmd, ' ', parts);
        //Serial.println("There were " + String(num) + " parts");
        // extract channel
        int pos = cmd.indexOf(' ', pos);
        int pos2 = cmd.indexOf(' ', pos+1);
        if (pos == -1 || pos2 == -1) break;
        channelPart = cmd.substring(pos, pos2);
  
        // extract value
        pos = pos2+1;
        valuePart = cmd.substring(pos);

        ok = true;
        break;
      }

      if (ok)
      {
        Serial.println("Setting channel " + channelPart + " to " + valuePart);
        channel = channelPart.toInt();
        value = valuePart.toFloat();

        
        
      } else {
        Serial.println("Invalid command syntax!");
      }

    }
	}
 
}

