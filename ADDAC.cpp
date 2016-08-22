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
	Serial.println("ADDAC Constructed");
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
	digitalWrite(ldacPin, state ? 1 : 0);
	//Serial.print("LDAC=");
	//Serial.println(state);
}


unsigned char ADDAC::Setup(uint8_t ldacPin)
{
	// only set up once?
	_setup = false;
	unsigned char i2cstatus;
	if (!_setup)
	{
		i2cstatus = I2C_Init(100000);

		ADDAC::ldacPin = ldacPin;
		pinMode(ldacPin, OUTPUT);
		digitalWrite(ldacPin, 0);
		_setup = true;

		Serial.println("Setup");
	}
	return i2cstatus;
}

ADDAC::ADDAC()
{
	Setup(ldacPin);
}

/***************************************************************************//**
* @brief Initializes the I2C communication peripheral.
*
* @param clockFreq - I2C clock frequency (Hz).
*                    Example: 100000 - I2C clock frequency is 100 kHz.
* @return status - Result of the initialization procedure.
*                  Example: 1 - if initialization was successful;
*                           0 - if initialization was unsuccessful.
*******************************************************************************/
unsigned char ADDAC::I2C_Init(unsigned long clockFreq)
{
	//Wire.begin(); Will allocate 160 bytes of memory, signal sampler OR this should be initializing I2C, not twice.
	Wire.begin();
	Wire.setClock(clockFreq);
	return 1;
}



/***************************************************************************//**
* @brief Writes data to a slave device.
*
* @param slaveAddress - Address of the slave device.
* @param dataBuffer - Pointer to a buffer storing the transmission data.
* @param bytesNumber - Number of bytes to write.
* @param stopBit - Stop condition control.
*                  Example: 0 - A stop condition will not be sent;
*                           1 - A stop condition will be sent.
*
* @return status - Number of written bytes.
																			 *******************************************************************************/

unsigned char ADDAC::I2C_Write(unsigned char slaveAddress,
	const char* dataBuffer,
	unsigned char bytesNumber,
	bool stopBit)
{
	unsigned char result;
	Wire.beginTransmission(slaveAddress);
	Wire.write(dataBuffer, bytesNumber);
	result = Wire.endTransmission(stopBit);
	return result;
}


/***************************************************************************//**
* @brief Reads data from a slave device.
*
* @param slaveAddress - Address of the slave device.
* @param dataBuffer - Pointer to a buffer that will store the received data.
* @param bytesNumber - Number of bytes to read.
* @param stopBit - Stop condition control.
*                  Example: 0 - A stop condition will not be sent;
*                           1 - A stop condition will be sent.
*
* @return status - Number of read bytes.
*******************************************************************************/
unsigned char ADDAC::I2C_Read(unsigned char slaveAddress,
	unsigned char* dataBuffer,
	unsigned char bytesNumber,
	bool stopBit)
{

	Wire.requestFrom((uint8_t)(slaveAddress), (uint8_t)(bytesNumber), (uint8_t)stopBit);    // request  bytes from slave device #

	while (Wire.available()) { // slave may send less than requested
							   //perhaps some delay here? might be nice.
		*dataBuffer = Wire.read();
		++dataBuffer;
	}
	return bytesNumber;
}

