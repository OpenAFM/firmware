// 
// 
// 

#include "ADDAC.h"

#define ON HIGH
#define OFF LOW

bool ADDAC::_setup;
uint8_t ADDAC::ldacPin;

ADDAC_Static_Constructor::ADDAC_Static_Constructor()
{
	ADDAC::ldacPin = 0;
	ADDAC::_setup = false;
}

void ADDAC::LoadDAC()
{
	digitalWrite(ldacPin, OFF);
	digitalWrite(ldacPin, ON);
}

void ADDAC::SetLDac(bool state)
{
	digitalWrite(ldacPin, state ? ON : OFF);
}


void ADDAC::Setup(uint8_t ldacPin)
{
	// only set up once?
	if (!_setup)
	{
		ADDAC::ldacPin = ldacPin;
		pinMode(ldacPin, OUTPUT);
		digitalWrite(ldacPin, OFF);
		_setup = true;
	}
}

ADDAC::ADDAC()
{
	Setup(ldacPin);
}