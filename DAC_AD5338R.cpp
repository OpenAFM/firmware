/*************************************************************************
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

/******************************************************************************/
/***************************** Include Files **********************************/
/******************************************************************************/
#include "DAC_AD5338R.h"

/******************************************************************************/
/************************ Functions Definitions *******************************/
/******************************************************************************/

/***************************************************************************//**
 * @brief Initializes the device.
 *
 * @param a1LogicLevel - Address pin A1.
 * @param a0LogicLevel - Address pin A0.
 *
 * @return status - Result of the initialization procedure.
 *          Example: 0x0 - I2C peripheral was not initialized.
 *                 0x1 - I2C peripheral is initialized.
*******************************************************************************/
unsigned char DAC_AD5338R::Init(unsigned char a1LogicLevel,
                          unsigned char a0LogicLevel)
{
    unsigned char status = 0;
    
    /* GPIO configuration. */
    pinMode(AD569X_LDAC_OUT, OUTPUT);
    digitalWrite(AD569X_LDAC_OUT, HIGH);
    pinMode(AD569X_RESET_OUT, OUTPUT);
    digitalWrite(AD569X_RESET_OUT, HIGH);
    pinMode(AD569X_RSTSEL_OUT, OUTPUT);
    digitalWrite(AD569X_RSTSEL_OUT, LOW);
    pinMode(AD569X_GAIN_OUT, OUTPUT);
    digitalWrite(AD569X_GAIN_OUT, LOW);

    status = I2C_Init(100000);
    
	deviceBitsNumber = 10;
    
    /* Store logic levels of A1 and A0 that set the two LSBs of the slave 
       address. */
       
    addressPinA1 = a1LogicLevel << 1;
    addressPinA0 = a0LogicLevel << 0;
    
    return status;
}

/***************************************************************************//**
 * @brief Puts the device in a specific power mode.
 *
 * @param channel - Channel option. Only one channel may be selected.
 *                  Example: AD569X_ADDR_DAC_A
 *                           AD569X_ADDR_DAC_B
 *                           AD569X_ADDR_DAC_C
 *                           AD569X_ADDR_DAC_D         
 * @param pwrMode - Power mode of the device.
 *                  Example: AD569X_PD_NORMAL   - Normal operation
 *                           AD569X_PD_1K       - 1 kOhm to GND
 *                           AD569X_PD_100K     - 100 kOhm to GND
 *                           AD569X_PD_3STATE   - Three-state
 *                               
 *
 * @return none.
*******************************************************************************/
void DAC_AD5338R::PowerMode(unsigned char channel, unsigned char pwrMode)
{    
    switch(channel)
    {
        case AD569X_ADDR_DAC_A:
            currentPowerRegValue &= ~AD569X_PWR_PDA(0x3);   // Clear power bits
            currentPowerRegValue |= AD569X_PWR_PDA(pwrMode);
            break;
        case AD569X_ADDR_DAC_B:
            currentPowerRegValue &= ~AD569X_PWR_PDB(0x3);
            currentPowerRegValue |= AD569X_PWR_PDB(pwrMode);
            break;
        case AD569X_ADDR_DAC_C:
            currentPowerRegValue &= ~AD569X_PWR_PDC(0x3);
            currentPowerRegValue |= AD569X_PWR_PDC(pwrMode);
            break;
        case AD569X_ADDR_DAC_D:
            currentPowerRegValue &= ~AD569X_PWR_PDD(0x3);
            currentPowerRegValue |= AD569X_PWR_PDD(pwrMode);
            break;
    }
    SetInputRegister(AD569X_CMD(AD569X_CMD_POWERMODE) | 
                             currentPowerRegValue);
}

/***************************************************************************//**
 * @brief Resets the device(clears the outputs to either zero scale or 
          midscale).
 *
 * @param resetOutput - The output values of the device at power-up or reset.
 *                      Example: 
 *                    AD569X_RST_ZERO_SCALE - outputs are cleared to zero scale.
 *                    AD569X_RST_MIDSCALE   - outputs are cleared to midscale.
 *
 * @return none.
*******************************************************************************/
void DAC_AD5338R::Reset(unsigned char resetOutput)
{
    if(resetOutput)
    {
        AD569X_RSTSEL_HIGH;
    }
    else
    {
        AD569X_RSTSEL_LOW;
    }
    SetInputRegister(AD569X_CMD(AD569X_CMD_SOFT_RESET));
}

/***************************************************************************//**
 * @brief Writes a 24-bit data-word to the Input Register of the device.
 *
 * @param registerValue - Value of the register.
 *
 * @return none.
*******************************************************************************/
void DAC_AD5338R::SetInputRegister(unsigned long registerValue)
{
    unsigned char registerWord[3] = {0, 0, 0};
    unsigned char* dataPointer    = (unsigned char*)&registerValue;

    registerWord[0] = dataPointer[2];
    registerWord[1] = dataPointer[1];
    registerWord[2] = dataPointer[0];

	const char outbuffer[3] = { dataPointer[2], dataPointer[1], dataPointer[0] };
    I2C_Write(AD569X_5MSB_SLAVE_ADDR | addressPinA1 | addressPinA0,
			  outbuffer,
              3, 
              1);
}

/***************************************************************************//**
 * @brief Select internal or external voltage reference.
 *
 * @param vRefMode - Voltage reference option.
 *                   Example: AD569X_INT_REF_ON  - Internal reference is used.
 *                            AD569X_INT_REF_OFF - External reference is used.
 *
 * @return none.
*******************************************************************************/
void DAC_AD5338R::InternalVoltageReference(unsigned char vRefMode)
{
    SetInputRegister(AD569X_CMD(AD569X_CMD_INT_REF_SETUP) | 
                            vRefMode);
}

/***************************************************************************//**
 * @brief Write data to the Input Register or to DAC Register of a channel.
 *
 * @param writeCommand - The write command option.
 *                       Example: 
*                        AD569X_CMD_WR_INPUT_N    - Write to Input Register n.
 *                       AD569X_CMD_WR_UPDT_DAC_N - Write to and update DAC n.
 * @param channel - Channel option. Any combination of DAC channels can be 
                    selected.
 *                  Example: AD569X_ADDR_DAC_A
 *                           AD569X_ADDR_DAC_B
 *                           AD569X_ADDR_DAC_C
 *                           AD569X_ADDR_DAC_D
 * @param data -  Data value to write.
 *
 * @return none.
*******************************************************************************/
void DAC_AD5338R::WriteFunction(unsigned char writeCommand, 
                          unsigned char channel, 
                          unsigned short data)
{
    unsigned char shiftValue = 0;
    
    /* Different types of devices have different data bits positions. */
    shiftValue = 16 - deviceBitsNumber;
    SetInputRegister(AD569X_CMD(writeCommand) |
                            AD569X_ADDR(channel) | 
                            ((long)AD569X_DATA_BITS(data) << shiftValue));
}

/***************************************************************************//**
 * @brief Reads back the binary value written to one of the channels.
 *
 * @param dacChannelAddr - Channel address.Channel option. Any combination of 
 *                         DAC channels can be selected.
 *                         Example: AD569X_ADDR_DAC_A
 *                                  AD569X_ADDR_DAC_B
 *                                  AD569X_ADDR_DAC_C
 *                                  AD569X_ADDR_DAC_D 
 *
 * @return 12-bit value of the selected channel.
*******************************************************************************/
unsigned short DAC_AD5338R::ReadBack(unsigned char dacChannelAddr)
{
    unsigned long channelValue = 0;
    unsigned char shiftValue   = 0;
    //unsigned char buffer[2]    = {0, 0};
    
    /* Different types of devices have different data bits positions. */
    shiftValue = 16 - deviceBitsNumber;
    //buffer[0] = dacChannelAddr;
	const char outbuffer[2] = { dacChannelAddr , 0 };
    I2C_Write(AD569X_5MSB_SLAVE_ADDR | addressPinA1 | addressPinA0,
			outbuffer,
              1,
              0);

	unsigned char inbuffer[2] = { 0, 0 };
    I2C_Read(AD569X_5MSB_SLAVE_ADDR | addressPinA1 | addressPinA0,
			inbuffer,
             2,
             1);
    
    channelValue = ((long)inbuffer[0] << 8) | inbuffer[1];
    channelValue >>= shiftValue;
    
    return channelValue;
}

/***************************************************************************//**
 * @brief Selects the output voltage of the selected channel.
 *
 * @param channel - Channel option. Channel option. Any combination of DAC 
 *                  channels can be selected.
 *                  Example: AD569X_ADDR_DAC_A
 *                           AD569X_ADDR_DAC_B
 *                           AD569X_ADDR_DAC_C
 *                           AD569X_ADDR_DAC_D
 * @param outputVoltage - Output voltage value.
 * @param vRef - Value of the voltage reference used. If GAIN pin is tied to Vdd
 *               vRef value is multiplied by 2 inside this function.
 *
 * @return The actual value of the output voltage.
*******************************************************************************/
float DAC_AD5338R::SetVoltage(unsigned char channel, 
                        float outputVoltage, 
                        float vRef)
{
    unsigned short binaryValue   = 0;
    float          actualVoltage = 0;
    
    if(vRef == 0)
    {
        vRef = 2.5;
    }
    vRef *= (AD569X_GAIN_STATE != 0) ? 2 : 1;
    binaryValue = (unsigned short)(outputVoltage * (1ul << deviceBitsNumber) / 
                                  vRef);
    WriteFunction(AD569X_CMD_WR_UPDT_DAC_N, channel, binaryValue);
    actualVoltage = (float)(vRef * binaryValue) / (1ul << deviceBitsNumber);
    
    return actualVoltage;
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

unsigned char DAC_AD5338R::I2C_Write(unsigned char slaveAddress,
                        const char* dataBuffer,
                        unsigned char bytesNumber,
                        bool stopBit)
{
    unsigned char result;
    Wire.beginTransmission(slaveAddress);
    Wire.write(dataBuffer,bytesNumber);
    result=Wire.endTransmission(stopBit);
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
unsigned char DAC_AD5338R::I2C_Read(unsigned char slaveAddress,
                       unsigned char* dataBuffer,
                       unsigned char bytesNumber,
                       bool stopBit)
{
 
  Wire.requestFrom((uint8_t)(slaveAddress),(uint8_t)(bytesNumber),(uint8_t)stopBit);    // request  bytes from slave device #
  
  while (Wire.available()) { // slave may send less than requested
    //perhaps some delay here? might be nice.
     *dataBuffer=Wire.read();
     ++dataBuffer;
  }
return bytesNumber;
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
unsigned char DAC_AD5338R::I2C_Init(unsigned long clockFreq)
{
  //Wire.begin(); Will allocate 160 bytes of memory, signal sampler OR this should be initializing I2C, not twice.
  Wire.setClock(clockFreq);
  return 1;
}
