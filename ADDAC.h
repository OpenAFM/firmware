// ADDAC.h
#include "stdafx.h"

#ifndef _ADDAC_h
#define _ADDAC_h

#if defined(ARDUINO) && ARDUINO >= 100
	#include "arduino.h"
#else
	#include "WProgram.h"
#endif

// this class only exists to instantiate static members of ADDAC class
class ADDAC_Static_Constructor
{
	ADDAC_Static_Constructor();
};

class ADDAC
{
	friend class ADDAC_Static_Constructor;

private:
	static bool _setup;
	static uint8_t ldacPin;
	static ADDAC_Static_Constructor scon;

public:

	// take the LDAC pin low, then high, to trigger DAQ register updates everywhere
	static void LoadDAC();
	static void SetLDac(bool state);
	ADDAC();
	static void Setup(uint8_t ldacPin);
};

#endif

