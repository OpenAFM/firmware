#include "stdafx.h"
#include "Scanner.h"
#include "RTx.h"
#include "DAC_AD5696.h"

/* Pin Definitions */

//Pins for DAC (tlc5620)
#define LDAC 7 //ldac HIGH to stop DAC output while clocking. 
#define RNG 0   //HIGH doubles the output voltage

//Imaging parameters
#define STEPSIZE 1      //Stepsize of DAC(linelength/stepsize=number of pixel per line)
#define LINE_LENGTH 10 //Voltage range over (MAX256). Defines size of image in real space
#define SAMPLE_SIZE 5   //Number of samples taken at each pixel. Median is taken as true value

//Communication parameters
#define BAUDRATE 9600   //Serial interfaces communication speed (bps)



//int splitString(String str, char delimiter, String *out)
//{
//    int len = str.length();
//    int pos = 0;
//    int pos2 = 0;
//    int count = 0;
//
//	// how many delimiters in string?
//	int numParts = 0;
//	for (int i = 0; i < len; i++)
//	{
//		if (str[i] == delimiter) numParts++;
//	}
//	numParts += 1;  // 1 delimiter means 2 parts
//	out = new String[numParts];
//
//	for (int i = 0; i < numParts; i++)
//    {
//      // find the next delimiter
//      pos2 = str.indexOf(delimiter, pos);
//
//      // ended?
//	  if (pos2 == -1)
//	  {
//		  String part = str.substring(pos);
//		  out[i] = part;
//		  break;
//	  }
//	  else
//	  {
//		  // extract the string
//		  String part = str.substring(pos, pos2);
//		  out[i] = part;
//	  }
//
//	  pos = pos2 + 1;
//    }
//	return numParts;
//}



/* Setup */
Adafruit_ADS1015 adc;
RTx* phone = new RTx();
DAC_AD5696* dac = new DAC_AD5696();
PiezoDACController* ctrl = new PiezoDACController(dac, STEPSIZE, LINE_LENGTH, LDAC, RNG);
SignalSampler* sampler = new SignalSampler(adc, SAMPLE_SIZE);
Scanner* scanner = new Scanner(*ctrl, *sampler, *phone, LINE_LENGTH);
//unsigned char zchar = (unsigned char)0;

//long dacMax = (long)5 * (long)65535;

//This function runs once, when the arduino starts
void setup() {
	Serial.begin(9600);
	Serial.print("Initialising I2C...");
	unsigned char i2csetup = ADDAC::Setup(LDAC);
	Serial.println(i2csetup == 1 ? "success!" : "failed!");

	dac->Init(0,0);
}

extern String const PARAM_LINE_LENGTH;

//This function keeps looping
void loop() 
{
	

	String cmd = phone->listen();

	//delay(1);
  int idx;

  //idx = cmd.indexOf("PDAC::REFSET");
  //Serial.println(idx);

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
     
		ctrl = new PiezoDACController(dac, CUSTOM_STEPSIZE, CUSTOM_LINE_LENGTH, LDAC, RNG);
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
	else if (cmd == "PDAC::RESET")
	{
		dac->Reset(AD569X_RST_MIDSCALE);
		Serial.println("Resetting Piezo DAC");
	}
	else if (cmd.indexOf("PDAC::REFSET") == 0)
	{
		Serial.println("asdasd");
		bool ok = false;
		String part;
		int val = 0;
		while (1)
		{
			int pos = cmd.indexOf(' ', pos);
			if (pos == -1) break;
			part = cmd.substring(pos+1);
			Serial.println(part);
			val = part.toInt();
			if (val != 0 && val != 1) break;

			ok = true;
			break;
		}
		if (ok)
		{
			Serial.print("Turning internal reference ");
			Serial.println(val == 1 ? "on" : "off");
			dac->InternalVoltageReference(val == 0 ? 1 : 0);
		}
		else
		{
			Serial.println("PDAC::REFSET - Invalid command syntax!");
		}
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
			while (1)
			{
				String *parts;
				//int num = splitString(cmd, ' ', parts);
				//Serial.println("There were " + String(num) + " parts");
				// extract channel
				int pos = cmd.indexOf(' ', pos);
				int pos2 = cmd.indexOf(' ', pos + 1);
				if (pos == -1 || pos2 == -1) break;
				channelPart = cmd.substring(pos, pos2);
				channel = channelPart.toInt();

				// check range
				if (channel < 1 || channel > 4)
				{
					Serial.println("Channel number must be 1, 2, 3 or 4");
					break;
				}

				// extract value
				pos = pos2 + 1;
				valuePart = cmd.substring(pos);
				value = valuePart.toFloat();

				// check range
				if (value < 0.0f || value > 5.0f)
				{
					Serial.println("Channel value must be between 0.0 and 5.0");
					break;
				}

				ok = true;
				break;
			}

			if (ok)
			{
				Serial.print("Setting channel ");
				Serial.print(channel);
				Serial.print(" to ");
				Serial.println(value);

				//long rand = random(0, dacMax);
				//float rnd = 5.0F;
				//rnd /= dacMax;
				//rnd *= rand;
				dac->SetVoltage(channel, value, 5.0f);



			}
			else {
				Serial.println("Invalid command syntax!");
			}

		}

		////////////////////////////////////////////
		// READ THE DAC VOLTAGE
		////////////////////////////////////////////
		else if (idx = cmd.indexOf("PDAC::GET") == 0)
		{
			//Serial.println("DACC!!!");
			bool ok = false;
			String channelPart;
			int channel;
			float value;
			while (1)
			{
				String *parts;
				//int num = splitString(cmd, ' ', parts);
				//Serial.println("There were " + String(num) + " parts");
				// extract channel
				int pos = cmd.indexOf(' ', pos);
				if (pos == -1) break;
				channelPart = cmd.substring(pos+1);
				channel = channelPart.toInt();

				// check range
				if (channel < 1 || channel > 4)
				{
					Serial.println("Channel number must be 1, 2, 3 or 4");
					break;
				}

				ok = true;
				break;
			}

			if (ok)
			{

				//long rand = random(0, dacMax);
				//float rnd = 5.0F;
				//rnd /= dacMax;
				//rnd *= rand;
				unsigned short val = dac->ReadBack((unsigned char)channel);
				Serial.print("Channel ");
				Serial.print(channel);
				Serial.print(" is set to ");
				Serial.println(val);



			}
			else {
				Serial.println("PDAC::GET - Invalid command syntax!");
			}

		}

		////////////////////////////////////////////
		// SET THE STATE OF THE LDAC PIN
		////////////////////////////////////////////
		else if (idx = cmd.indexOf("LDAC::SET") == 0)
		{
			//Serial.println("DACC!!!");
			bool ok = false;
			String channelPart;
			int state;
			float value;
			while (1)
			{
				String *parts;
				//int num = splitString(cmd, ' ', parts);
				//Serial.println("There were " + String(num) + " parts");
				// extract channel
				int pos = cmd.indexOf(' ', pos);
				if (pos == -1) break;
				channelPart = cmd.substring(pos + 1);
				state = channelPart.toInt();

				// check range
				if (state != 0 && state != 1)
				{
					Serial.println("Must be 0 or 1");
					break;
				}

				ok = true;
				break;
			}

			if (ok)
			{

				//long rand = random(0, dacMax);
				//float rnd = 5.0F;
				//rnd /= dacMax;
				//rnd *= rand;
				ADDAC::SetLDac(state == 1);
				Serial.print("LDAC state was set to ");
				Serial.println(state == 1 ? "ON" : "OFF");



			}
			else {
				Serial.println("LDAC::SET - Invalid command syntax!");
			}

		}
	}
 
}

