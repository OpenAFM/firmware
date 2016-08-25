//#include <Adafruit_ADS1015.h>
#include "stdafx.h"
#include "Scanner.h"
#include "RTx.h"
#include "DAC_AD5696.h"


// commands
//#define ADCCOMMANDS

/* Pin Definitions */

//Pins for DAC (tlc5620)
#define LDAC 7 //ldac HIGH to stop DAC output while clocking. 
#define RNG 0   //HIGH doubles the output voltage

//Imaging parameters
#define STEPSIZE 1      //Stepsize of DAC(linelength/stepsize=number of pixel per line)
#define LINE_LENGTH 10 //Voltage range over (MAX256). Defines size of image in real space
#define SAMPLE_SIZE 5   //Number of samples taken at each pixel. Median is taken as true value

//Communication parameters
#define BAUDRATE 250000   //Serial interfaces communication speed (bps)

bool reply = true;


/*!
	Check <commandLine> for something of the form "<name> <number>"
	If it is found, extract the <numer> and put it in <param>, and return true
*/
bool CheckSingleParameter(String commandLine, String name, int &param, bool &ok, String errorMessage)
{
	Serial.println(commandLine);
	Serial.println(name);
	Serial.println(commandLine.indexOf(name));
	if (commandLine.indexOf(name) == 0)
	{
		Serial.println("Found....");
		// setting
		bool ok = false;
		String part;
		int val = 0;
		while (1)
		{
			int pos = commandLine.indexOf(' ', pos);
			if (pos == -1) break;
			part = commandLine.substring(pos + 1);
			//Serial.println(part);
			val = part.toInt();
			if (val < 0) break;

			ok = true;
			break;
		}

		if (ok)
		{
			Serial.println("OK");
			param = val;
			Serial.println(param);
		}
		else
		{
			if (reply) Serial.println(errorMessage);
		}

		return true;
	}
	return false;
}


/* Setup */
Adafruit_ADS1015 sig_adc(0x49);   // adc with raw signal input (A, B, C and D)
Adafruit_ADS1015 diff_adc(0x48);   // adc with the sum and difference signals
RTx* phone = new RTx();
DAC_AD5696* vc_dac = new DAC_AD5696();
//DAC_AD5696* vcdac = new DAC_AD5696();
PiezoDACController* ctrl = new PiezoDACController(vc_dac, STEPSIZE, LINE_LENGTH, LDAC, RNG);
SignalSampler* sampler = new SignalSampler(diff_adc, SAMPLE_SIZE);
Scanner* scanner = new Scanner(*ctrl, *sampler, *phone, LINE_LENGTH);


//This function runs once, when the arduino starts
void setup() {
	Serial.begin(BAUDRATE);
	Serial.print("Initialising I2C...");
	unsigned char i2csetup = ADDAC::Setup(LDAC);
	Serial.println(i2csetup == 1 ? "success!" : "failed!");

	vc_dac->Init(10, 1, 1);

	// turn internal reference off
	vc_dac->InternalVoltageReference(AD569X_INT_REF_OFF);

	// start ADCs
	diff_adc.begin();
}

extern String const PARAM_LINE_LENGTH;

//This function keeps looping
void loop()
{


	String cmd = phone->listen();

	//delay(1);
	int idx;
	bool boolean;

	//idx = cmd.indexOf("VCDAC::REFSET");
	//Serial.println(idx);

	 /*
	  * Get command over serial
	  * VCDAC::SET <x> <y>
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

		ctrl = new PiezoDACController(vc_dac, CUSTOM_STEPSIZE, CUSTOM_LINE_LENGTH, LDAC, RNG);
		sampler = new SignalSampler(sig_adc, CUSTOM_SAMPLE_SIZE);
		scanner = new Scanner(*ctrl, *sampler, *phone, CUSTOM_LINE_LENGTH);

		if (reply)
		{
			Serial.print("Setup with STEP_SIZE= ");
			Serial.print(CUSTOM_STEPSIZE);
			Serial.print(", CUSTOM_LINE_LENGTH= ");
			Serial.print(CUSTOM_LINE_LENGTH);
			Serial.print(" CUSTOM_SAMPLE_SIZE= ");
			Serial.println(CUSTOM_SAMPLE_SIZE);
		}

	}
	else if (cmd == "STREAM")
	{
		scanner->stream();
	}

	else if (cmd == "SCAN::STARTINGXPLUS::GET")
	{
		Serial.println(ctrl->startingXPlus);
	}
	else if (CheckSingleParameter(cmd, "SCAN::STARTINGXPLUS::SET", idx, boolean, "SCAN::STARTINGXPLUS - Invalid command syntax!"))
	{
		Serial.print("Setting startingXPlus to ");
		Serial.println(idx);
	}


	////////////////
	//// LINE LENGTH
	////////////////
	//else if (cmd == "SCAN::LINELENGTH::GET")
	//{
	//	Serial.println(ctrl->getLineSize());
	//}
	//else if (cmd.indexOf("SCAN::LINELENGTH::SET") == 0)
	//{
	//	// setting
	//	bool ok = false;
	//	String part;
	//	int val = 0;
	//	while (1)
	//	{
	//		int pos = cmd.indexOf(' ', pos);
	//		if (pos == -1) break;
	//		part = cmd.substring(pos + 1);
	//		//Serial.println(part);
	//		val = part.toInt();
	//		if (val < 0) break;

	//		ok = true;
	//		break;
	//	}
	//	if (ok)
	//	{
	//		if (reply)
	//		{
	//			Serial.print("Setting line length to ");
	//			Serial.println(val);
	//		}
	//		ctrl->setLineSize(val);
	//	}
	//	else
	//	{
	//		if (reply) Serial.println("SCAN::LINELENGTH::SET - Invalid command syntax!");
	//	}
	//}


	//////////////
	// STEP SIZE
	//////////////
	else if (cmd == "SCAN::STEPSIZE::GET")
	{
		//Serial.print("LineLength is ");
		Serial.println(ctrl->getStepSize());
	}
	else if (CheckSingleParameter(cmd, "SCAN::STEPSIZE::SET", idx, boolean, "SCAN::STEPSIZE::SET - Invalid command syntax!"))
	{
		if (reply)
		{
			Serial.print("Setting step size to ");
			Serial.println(idx);
		}
		ctrl->setStepSize(idx);
	}
	//else if (cmd.indexOf("SCAN::STEPSIZE::SET") == 0)
	//{
	//	// setting
	//	bool ok = false;
	//	String part;
	//	int val = 0;
	//	while (1)
	//	{
	//		int pos = cmd.indexOf(' ', pos);
	//		if (pos == -1) break;
	//		part = cmd.substring(pos + 1);
	//		//Serial.println(part);
	//		val = part.toInt();
	//		if (val < 0) break;

	//		ok = true;
	//		break;
	//	}
	//	if (ok)
	//	{
	//		if (reply)
	//		{
	//			Serial.print("Setting step size to ");
	//			Serial.println(val);
	//		}
	//		ctrl->setStepSize(val);
	//	}
	//	else
	//	{
	//		if (reply) Serial.println("SCAN::STEPSIZE::SET - Invalid command syntax!");
	//	}
	//}



	else if (cmd == "ERROR")
	{

	}


	else if (cmd == "PING")
	{
		Serial.println("PONG");
	}
	else if (cmd == "REPLY")
	{
		reply = true;
	}
	else if (cmd == "NOREPLY")
	{
		reply = false;
	}
	else if (CheckSingleParameter(cmd, "ECHO", idx, boolean, "ECHO - Invalid command syntax!"))
	{
		phone->echo = idx == 1;
		//Serial.print("Echo is ");
		//Serial.println(idx);
	}
	/*else if (cmd == "ECHO")
	{
		phone->echo = true;
	}
	else if (cmd == "NOECHO")
	{
		phone->echo = false;
	}*/


	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	/// ADC COMMANDS
	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#ifdef ADCCOMMANDS

	else if (idx = cmd.indexOf("ADC") == 0)
	{
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/// GET
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		if (cmd.indexOf("::GET"))
		{
			//Serial.println("Measuring ADC");
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
				channelPart = cmd.substring(pos + 1);
				channel = channelPart.toInt();

				// check range
				if (channel < 1 || channel > 4)
				{
					if (reply) Serial.println("Channel number must be 1, 2, 3 or 4 (for A, B, C or D)");
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
				int16_t val;

				// which adc?
				if (cmd.indexOf("ADCDIFF::GET") == 0)
				{
					val = diff_adc.readADC_SingleEnded(channel + 1);
					//Serial.print("Channel ");
					//Serial.print(channel);
					//Serial.print(" of diff_adc is ");
				}
				else if (cmd.indexOf("ADCSIG::GET") == 0)
				{
					val = sig_adc.readADC_SingleEnded(channel + 1);
					//Serial.print("Channel ");
					//Serial.print(channel);
					//Serial.print(" of sig_adc is ");
				}
				Serial.println(val);



			}
			else {
				Serial.println("AD...::GET - Invalid command syntax!");
			}
		}

	}

#endif

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	/// DAC COMMANDS
	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#ifdef DACCOMMANDS

	else if (cmd == "VCDAC::PRINT")
	{
		Serial.print("Max value (u) = ");
		Serial.println(vc_dac->getMaxValueU());
		Serial.print("Max value (f) = ");
		Serial.println(vc_dac->getMaxValueF());
		Serial.print("Bits = ");
		Serial.println(vc_dac->getBits());
	}
	else if (cmd == "VCDAC::RESET")
	{
		vc_dac->Reset(AD569X_RST_MIDSCALE);
		if (reply) Serial.println("Resetting Piezo DAC");
	}
	else if (cmd.indexOf("VCDAC::REFSET") == 0)
	{
		bool ok = false;
		String part;
		int val = 0;
		while (1)
		{
			int pos = cmd.indexOf(' ', pos);
			if (pos == -1) break;
			part = cmd.substring(pos + 1);
			Serial.println(part);
			val = part.toInt();
			if (val != 0 && val != 1) break;

			ok = true;
			break;
		}
		if (ok)
		{
			if (reply)
			{
				Serial.print("Turning internal reference ");
				Serial.println(val == 1 ? "on" : "off");
			}
			vc_dac->InternalVoltageReference(val == 0 ? AD569X_INT_REF_OFF : AD569X_INT_REF_ON);
		}
		else
		{
			if (reply) Serial.println("VCDAC::REFSET - Invalid command syntax!");
		}
	}
	else
	{
		//////////////////////////////////////////////////////
		// SET DAC VOLTAGE (FOR DEBUG)
		//////////////////////////////////////////////////////

		if (idx = cmd.indexOf("VCDAC::SET") == 0)
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
					if (reply) Serial.println("Channel number must be 1, 2, 3 or 4");
					break;
				}

				// extract value
				pos = pos2 + 1;
				valuePart = cmd.substring(pos);
				value = valuePart.toFloat();

				// check range
				if (value < 0.0f || value > 5.0f)
				{
					if (reply) Serial.println("Channel value must be between 0.0 and 5.0");
					break;
				}

				ok = true;
				break;
			}

			if (ok)
			{
				if (reply)
				{
					Serial.print("Setting channel ");
					Serial.print(channel);
					Serial.print(" to ");
					Serial.println(value);
				}

				//long rand = random(0, dacMax);
				//float rnd = 5.0F;
				//rnd /= dacMax;
				//rnd *= rand;
				vc_dac->SetVoltage(channel, value, 5.0f);
				//dac->SetOutput(1U << (channel - 1), 



			}
			else {
				if (reply) Serial.println("Invalid command syntax!");
			}

		}

		////////////////////////////////////////////
		// READ THE DAC VOLTAGE
		////////////////////////////////////////////
		else if (idx = cmd.indexOf("VCDAC::GET") == 0)
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
				channelPart = cmd.substring(pos + 1);
				channel = channelPart.toInt();

				// check range
				if (channel < 1 || channel > 4)
				{
					if (reply) Serial.println("Channel number must be 1, 2, 3 or 4");
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
				unsigned short val = vc_dac->ReadBack((unsigned char)channel);
				if (reply)
				{
					Serial.print("Channel ");
					Serial.print(channel);
					Serial.print(" is set to ");
					Serial.println(val);
				}



			}
			else {
				if (reply) Serial.println("VCDAC::GET - Invalid command syntax!");
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
				if (reply)
				{
					Serial.print("LDAC state was set to ");
					Serial.println(state == 1 ? "ON" : "OFF");
				}



			}
			else {
				if (reply) Serial.println("LDAC::SET - Invalid command syntax!");
			}

		}
	}
#endif

}

