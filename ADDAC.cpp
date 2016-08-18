// 
// 
// 

#include "ADDAC.h"

#define ON HIGH
#define OFF LOW

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


void ADDAC::Setup(int ldacPin)
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