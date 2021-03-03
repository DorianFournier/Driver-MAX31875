/*
 * MAX31875_Driver.h
 *
 *  Created on: Nov 23, 2020
 *      Author: dorian
 */

#ifndef INC_MAX31875_DRIVER_H_
#define INC_MAX31875_DRIVER_H_

#include "stm32f4xx_hal.h"
#include <stdbool.h>
#include <string.h>

/* Define Device Address */
#define MAX31875_DEV_ADDR 					0x90			// slave address

/* Define registers address */
#define MAX31875_TEMP_REG_ADDR 				0x00			// temperature register address
#define MAX31875_CONF_REG_ADDR 				0x01			// configuration register address

/* Define configuration parameters */							// look on MAX31875 datasheet

/* MAX31875 One-shot */
#define MAX31875_ONESHOT_DISABLE 			((uint8_t)0x00)
#define MAX31875_ONESHOT_ENABLE 			((uint8_t)0x01)

/* MAX31875 Conversion Rate */
#define MAX31875_CONVERSIONRATE_0_25		((uint8_t)0x00)		/* 0.25 conv/sec */
#define MAX31875_CONVERSIONRATE_1			((uint8_t)0x02)		/* 1 	conv/sec */
#define MAX31875_CONVERSIONRATE_4			((uint8_t)0x04)		/* 4	conv/sec */
#define MAX31875_CONVERSIONRATE_8			((uint8_t)0x06)		/* 8	conv/sec */

/* MAX31875 ShutDown */
#define MAX31875_SHUTDOWN_OFF				((uint8_t)0x00)		/* D8 -> 0 : continuous conversion on  */
#define MAX31875_SHUTDOWN_ON				((uint8_t)0x01)		/* D8 -> 1 : continuous conversion off */

/* MAX31875 TimeOut */
#define MAX31875_TIMEOUT_ENABLE				((uint8_t)0x00)
#define MAX31875_TIMEOUT_DISABLE			((uint8_t)0x10)

/* MAX31875 Resolution */
#define MAX31875_RESOLUTION_8				((uint8_t)0x00)		/* 8  bits of resolution */
#define MAX31875_RESOLUTION_9				((uint8_t)0x20)		/* 9  bits of resolution */
#define MAX31875_RESOLUTION_10				((uint8_t)0x40)		/* 10 bits of resolution */
#define MAX31875_RESOLUTION_12				((uint8_t)0x60)		/* 12 bits of resolution */

/* MAX31875 DataFormat */
#define MAX31875_DATAFORMAT_NORMAL			((uint8_t)0x00)		/* Normal mode   */
#define MAX31875_DATAFORMAT_EXTENDED		((uint8_t)0x80)		/* Extended mode */

/* Init Struct */
typedef struct						// structure declaration for configuration
{									// use typedef for rename this structure into 'MAX31875_Init_t'
	uint8_t conversionRate;
	uint8_t shutDown;
	uint8_t timeOut;
	uint8_t resolution;
	uint8_t dataFormat;
}MAX31875_Init_t;

/* Write Function (MAX31875_Write_Reg)*/
void MAX31875_Write_Reg(uint8_t reg, uint8_t *dataW, uint8_t size);

/* Read Function (MAX31875_Read_Reg)*/
void MAX31875_Read_Reg(uint8_t reg, uint8_t *dataR, uint8_t size);

/* Init Function (MAX31875_Init)*/
void MAX31875_Init(I2C_HandleTypeDef *tempI2C, MAX31875_Init_t *tempInitDef);

/* Get Temperature (MAX31875_Get_Temp)*/
float MAX31875_Get_Temp(void);

#endif /* INC_MAX31875_DRIVER_H_ */
