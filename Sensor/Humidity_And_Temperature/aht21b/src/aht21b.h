/**
 * @file aht21b.h
 * @brief Driver for AHT21B temperature and humidity sensor
 *
 * This file contains the implementation for AHT21B sensor interfacing functions.
 *
 * @date 2025-01-18
 * @author jainr
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

#define AHT21B_DATA_LEN						6u
#define AHT21B_DATA_BITS					8u
#define AHT21B_DATA_CRC_POS					7u
#define AHT21B_CRC_INIT						0xFF
#define AHT21B_CRC_POLY						0x31
#define AHT21B_8TH_BIT_MASK					0x80


/* Enums ----------------------------------------------*/

/* Structures -----------------------------------------*/

/* Variables ------------------------------------------*/

/* Function Declaration -------------------------------*/

/**
 * @brief Initializes the AHT21B sensor.
 *
 * @return e_Status STATUS_OK if successful, STATUS_NOT_OK otherwise.
 */
e_Status AHT21B_Init();

/**
 * @brief Reads the temperature and humidity values from the AHT21B sensor.
 *
 * @param[out] humidityVal Pointer to store the calculated humidity value.
 * @param[out] tempVal Pointer to store the calculated temperature value.
 * @return e_Status STATUS_OK if successful, STATUS_NOT_OK otherwise.
 */
e_Status AHT21B_GetTempHumidity(float *humidityVal, float *tempVal);



#endif /* AHT21B_H_ */
