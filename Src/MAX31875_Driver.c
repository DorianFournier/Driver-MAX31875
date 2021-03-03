/*
 * MAX31875_Driver.c
 *
 *  Created on: Nov 23, 2020
 *      Author: dorian
 */

#include "MAX31875_Driver.h"														// add MAX31875_Driver.h file

static I2C_HandleTypeDef tempI2C_Handle;											// declaration of static variable
static bool extended_Enable = false;												// declaration & initialization of static boolean variable
//static uint8_t oneShot = MAX31875_ONESHOT_ENABLE;

/* Convertion temp table */
const float convertion_temp_table[12] = {0.0625f, 0.125f, 0.25f, 0.5f, 1.0f, 2.0f, 4.0f, 8.0f, 16.0f, 32.0f, 64.0f, 128.0f};

/* MAX31875 Write Reg */
void MAX31875_Write_Reg(uint8_t reg, uint8_t *dataW, uint8_t size)					// use into MAX31875_Init function
{
	dataW[0]= reg;																	// add register address in the first case of dataW array
	HAL_I2C_Master_Transmit(&tempI2C_Handle, MAX31875_DEV_ADDR, dataW, size, 10);	// transmit dataW array
}

/* MAX31875 Read Reg */
void MAX31875_Read_Reg(uint8_t reg, uint8_t *dataR, uint8_t size)					// use into MAX31875_Get_Temp function
{
	HAL_I2C_Master_Transmit(&tempI2C_Handle, MAX31875_DEV_ADDR, &reg, 1, 10);		// send register Address
	HAL_I2C_Master_Receive(&tempI2C_Handle, MAX31875_DEV_ADDR, dataR, size, 10);	// receive dataR from Register
}

/* Init Function */
void MAX31875_Init(I2C_HandleTypeDef *tempI2C, MAX31875_Init_t *tempInitDef)
{
	uint8_t I2CData[3];															// declaration of uint8 data array
	memcpy(&tempI2C_Handle, tempI2C, sizeof(*tempI2C));							// copy data from memory area tempI2C to memory area tempI2C_Handle

	/* Configuration register */
	// Set configuration register settings value for upper byte					// used logical AND for mask data
	I2CData[1] |= (tempInitDef-> shutDown & 0x01);								// used logical OR to keep other bit unchanged

	// Set configuration register settings value for lower byte
	I2CData[2] |= (tempInitDef-> conversionRate & 0x06);
	I2CData[2] |= (tempInitDef-> timeOut & 0x10);
	I2CData[2] |= (tempInitDef-> resolution & 0x60);
	I2CData[2] |= (tempInitDef-> dataFormat & 0x80);

	MAX31875_Write_Reg(MAX31875_CONF_REG_ADDR, I2CData, 3);						// call MAX31875 Write function

	/*
	if (tempInitDef->shutDown == MAX31875_SHUTDOWN_ON)
	{
		oneShot = MAX31875_ONESHOT_ENABLE;
	}
	*/

	if(tempInitDef->dataFormat == MAX31875_DATAFORMAT_EXTENDED)					// check if extended DataFormat is enabled
	{
		extended_Enable = true;													// if it's enabled : extended_Enable become true
	}
}

/* Get Temperature */
float MAX31875_Get_Temp(void)
{
	float tempData = 0.0f;
	uint8_t var[2];
	uint16_t data = 0;

	MAX31875_Read_Reg(MAX31875_TEMP_REG_ADDR, var, 2);									// call MAX31875 Read function

	data = (var[0] << 8) | var[1];

	if(extended_Enable == true)
	{
		for(uint8_t n = 3 ; n<15 ; n++)													// n = 3 -> because D0, D1, D2 = 0
		{
			tempData = tempData + ((data >> n) & 1) * convertion_temp_table[n-3];
		}

	} else {
		for(uint8_t n = 4 ; n<15 ; n++)													// n = 4 -> because D0 = D1 = D2 = D3 = 0
		{
			tempData = tempData + ((data >> n) & 1) * convertion_temp_table[n-4];
		}
	}

	if( ((data >> 15) & 1) == 1 )														// check the sign bit (MSB)
	{
		tempData = -tempData;															// if MSB = 1 -> data is negative
	}
	else {
		tempData = tempData;															// if MSB = 0 -> data is positive
	}

	return tempData;
}

