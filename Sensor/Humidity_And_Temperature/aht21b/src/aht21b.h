/*
 * aht21b.h
 *
 *  Created on: Jan 18, 2025
 *      Author: jainr
 */

#ifndef AHT21B_H_
#define AHT21B_H_

/* Includes -------------------------------------------*/
#include <common.h>

/* Macro Definition -----------------------------------*/
#define AHT21B_7BIT_I2C_ADDRESS				0x38
#define AHT21B_I2C_WRITE_ADDRESS			(AHT21B_7BIT_I2C_ADDRESS << 1)
#define AHT21B_I2C_READ_ADDRESS				(AHT21B_I2C_WRITE_ADDRESS + 1)


#define AHT21B_STATUS_ADDRESS				0x71
#define AHT21B_STATUS_SIZE					1u
#define AHT21B_STATUS_CONST					0x18
#define AHT21B_STATUS_BUSY					0x80

#define AHT21B_REGISTER_A_ADDRESS			0x1B
#define AHT21B_REGISTER_B_ADDRESS			0x1C
#define AHT21B_REGISTER_C_ADDRESS			0x1E
#define AHT21B_REGISTER_ARRAY_SIZE			3u

#define AHT21B_START_MEASUREMENT			0xAC
#define AHT21B_MEASUREMENT_BYTE0			0x33
#define AHT21B_MEASUREMENT_BYTE1			0x00
#define AHT21B_MEASUREMENT_SIZE				0x2
#define AHT21B_MEASUREMENT_TIMEOUT			100u




/* Enums ----------------------------------------------*/

/* Structures -----------------------------------------*/

/* Variables ------------------------------------------*/

/* Function Declaration -------------------------------*/

e_Status AHT21B_Init();
e_Status AHT21B_GetTempHumidity();



#endif /* AHT21B_H_ */
