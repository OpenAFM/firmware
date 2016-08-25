#include "ADDAC.h"


/***************************************************************************//**
 * Copyright 2012(c) Analog Devices, Inc.
 *
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *  - Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 *  - Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in
 *    the documentation and/or other materials provided with the
 *    distribution.
 *  - Neither the name of Analog Devices, Inc. nor the names of its
 *    contributors may be used to endorse or promote products derived
 *    from this software without specific prior written permission.
 *  - The use of this software may or may not infringe the patent rights
 *    of one or more patent holders.  This license does not release you
 *    from the requirement that you obtain separate licenses from these
 *    patent holders to use this software.
 *  - Use of the software either in source or binary form, must be run
 *    on or directly connected to an Analog Devices Inc. component.
 *
 * THIS SOFTWARE IS PROVIDED BY ANALOG DEVICES "AS IS" AND ANY EXPRESS OR
 * IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, NON-INFRINGEMENT,
 * MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED.
 * IN NO EVENT SHALL ANALOG DEVICES BE LIABLE FOR ANY DIRECT, INDIRECT,
 * INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
 * LIMITED TO, INTELLECTUAL PROPERTY RIGHTS, PROCUREMENT OF SUBSTITUTE GOODS OR
 * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
 * CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
 * OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
********************************************************************************
*   SVN Revision: 776
*******************************************************************************/
#ifndef __AD5696_H__
#define __AD5696_H__


#if defined(ARDUINO) && ARDUINO >= 100
  #include "arduino.h"
#else
  #include "WProgram.h"
#endif

/******************************************************************************/
/***************************** Include Files **********************************/
/******************************************************************************/

#include <Wire.h>

/******************************************************************************/
/******************************** AD569X *************************************/
/******************************************************************************/


/* LDAC */
#define AD569X_LDAC_OUT        10
#define AD569X_LDAC_LOW        LOW
#define AD569X_LDAC_HIGH       HIGH
/* RESET */
#define AD569X_RESET_OUT       11
#define AD569X_RESET_LOW       LOW
#define AD569X_RESET_HIGH      HIGH
/* RSTSEL */
#define AD569X_RSTSEL_OUT      12
#define AD569X_RSTSEL_LOW      LOW
#define AD569X_RSTSEL_HIGH     HIGH
/* GAIN */
#define AD569X_GAIN_OUT        13
#define AD569X_GAIN_LOW        LOW
#define AD569X_GAIN_HIGH       HIGH
#define AD569X_GAIN_STATE      LOW

/* I2C slave address */
#define AD569X_5MSB_SLAVE_ADDR          0x0C

/* Logic states of A1 and A0 pins. */
#define AD569X_A1_LOW       0
#define AD569X_A1_HIGH      1
#define AD569X_A0_LOW       0
#define AD569X_A0_HIGH      1

/* AD569X Input Register */
#define AD569X_CMD(x)                  ((0x000F & (long)(x)) << 20)
#define AD569X_ADDR(x)                 ((0x000F & (long)(x)) << 16)
#define AD569X_DATA_BITS(x)            ((0xFFFF & (long)(x)) <<  0)

/* AD569X_CMD(x) options */
#define AD569X_CMD_NOP                 0 // No operation.
#define AD569X_CMD_WR_INPUT_N          1 // Write to Input Register n.
#define AD569X_CMD_UPDATE_DAC_N        2 // Update DAC Register n.
#define AD569X_CMD_WR_UPDT_DAC_N       3 // Write to and update DAC Channel n.
#define AD569X_CMD_POWERMODE           4 // Power down/power up DAC.
#define AD569X_CMD_LDAC_MASK           5 // Hardware LDAC mask register.
#define AD569X_CMD_SOFT_RESET          6 // Software reset(power-on reset).
#define AD569X_CMD_INT_REF_SETUP       7 // Internal reference setup register.

/* AD569X_ADDR(x) options */
#define AD569X_ADDR_DAC_A          1  // DAC A
#define AD569X_ADDR_DAC_B          2  // DAC B
#define AD569X_ADDR_DAC_C          4  // DAC C
#define AD569X_ADDR_DAC_D          8  // DAC D
#define AD569X_ADDR_DAC_ALL        15 // All DACs. Any combination of DAC 
                                      // channels can be selected using the 
                                      // address bits.

/* Power Setup Register definition */
#define AD569X_PWR_PDD(x)      (((x) & 0x3) << 6)
#define AD569X_PWR_PDC(x)      (((x) & 0x3) << 4)
#define AD569X_PWR_PDB(x)      (((x) & 0x3) << 2)
#define AD569X_PWR_PDA(x)      (((x) & 0x3) << 0)


/* AD569X_PWR_PDn(x) options(n = A, B, C, D) */
#define AD569X_PD_NORMAL       0 // Normal operation
#define AD569X_PD_1K           1 // 1 kOhm to GND
#define AD569X_PD_100K         2 // 100 kOhm to GND
#define AD569X_PD_3STATE       3 // Three-state

/* LDAC Mask Register definition */
#define AD569X_MSK_CH_A        1
#define AD569X_MSK_CH_B        2
#define AD569X_MSK_CH_C        4
#define AD569X_MSK_CH_D        8

/* Reference Setup Register definition */
#define AD569X_INT_REF_ON      0
#define AD569X_INT_REF_OFF     1

/* Clear code options */
#define AD569X_RST_ZERO_SCALE     0
#define AD569X_RST_MIDSCALE       1

#define AD569X_DAC_MAX 65535

/******************************************************************************/
/************************ Functions Declarations ******************************/
/******************************************************************************/
class DAC_AD5696 : public ADDAC
{
public:

	/*! Initializes the device. */
	unsigned char Init(unsigned char numBits, unsigned char a1LogicLevel,
		unsigned char a0LogicLevel);

	/*! Resets the device(clears the outputs to either zero scale or midscale). */
	void Reset(unsigned char resetOutput);

	/*! Puts the device in a specific power mode. */
	void PowerMode(unsigned char channel, unsigned char pwrMode);

	/*! Selects internal or external voltage reference. */
	void InternalVoltageReference(unsigned char vRefMode);

	/*!  Writes a 24-bit data-word to the Input Register of the device. */
	void SetInputRegister(unsigned long registerValue);

	/*! Writes data to the Input Register or to DAC Register of a channel. */
	void WriteFunction(unsigned char writeCommand,
		unsigned char channel,
		unsigned short data);

	/*! Reads back the binary value written to one of the channels. */
	unsigned short ReadBack(unsigned char dacChannelAddr);

	/*! Selects the output voltage of the selected channel. */
	float SetVoltage(unsigned char channel,
		float outputVoltage,
		float vRef);

	/*! Set the output value of a channel by binary. */
	int SetOutput(uint8_t channel, uint16_t value);

	uint16_t getMaxValueU() { return maxValue; }
	float getMaxValueF() { return maxValueF; }
	unsigned char getBits() { return deviceBitsNumber; }

private:


	/******************************************************************************/
	/************************ Variables Definitions *******************************/
	/******************************************************************************/
	unsigned char currentPowerRegValue = 0;
	unsigned char deviceBitsNumber = 0;
	unsigned char addressPinA1 = 0;
	unsigned char addressPinA0 = 0;
	uint16_t maxValue = 0;
	float maxValueF = 0;

};

#endif // __AD569X_H__
