/*
 * bmp180.h
 *
 *  Created on: Dec 17, 2024
 *      Author: jainr
 */

#ifndef BMP180_H_
#define BMP180_H_

#include <common.h>

#define BMP180_READ_ADDRESS				0xEF
#define BMP180_WRITE_ADDRESS			0xEE

#define BMP180_SOFT_RESET_REGISTER		0xE0
#define BMP180_SOFT_RESET_VALUE			0xB6

#define BMP180_CALIBRATION_REGISTER		0XAA
#define BMP180_CALIBRATION_SIZE			22u

#define BMP180_CONTROL_REGISTER			0xF4
#define BMP180_OUT_MSB_REGISTER			0xF6
#define BMP180_OUT_LSB_REGISTER			0xF7
#define BMP180_OUT_XLSB_REGISTER		0xF8

#define BMP180_TEMPERATURE_START		0x2E
#define BMP180_PRESSURE_OVERSAMPLING_0	0x34
#define BMP180_PRESSURE_OVERSAMPLING_1	0x74
#define BMP180_PRESSURE_OVERSAMPLING_2	0xB4
#define BMP180_PRESSURE_OVERSAMPLING_3	0xF4
#define BMP180_PRESSURE_START			0x34

#define BMP180_WAIT_TIME				5u

typedef enum e_SamplingMode
{
	ULTRA_LOW_POWER = 0x00,
	STANDARD,
	HIGH_RESOLUTION,
	ULTRA_HIGH_RESOLUTION
}e_SamplingMode;

typedef struct st_CalibrationCoeff
{
	/* Calibrated Coefficient from sensor*/
	int16_t   AC1;
	int16_t   AC2;
	int16_t   AC3;
	uint16_t  AC4;
	uint16_t  AC5;
	uint16_t  AC6;
	int16_t   B1;
	int16_t   B2;
	int16_t   MB;
	int16_t   MC;
	int16_t   MD;

	/* Calculated Variables */
	int32_t   B3;
	uint32_t  B4;
	int32_t   B5;
	int32_t   B6;
	uint32_t  B7;

	/* Sampling Mode */
	e_SamplingMode samplingMode;
}st_CalibrationCoeff;

/*
 * @brief  Initializes the BMP180 sensor.
 * @note   Performs a soft reset, checks device readiness, reads calibration coefficients, and sets the sampling mode.
 * @param  None
 * @retval e_Status  Status of the operation (STATUS_OK or STATUS_NOT_OK).
 */
e_Status BMP180_Init();

/*
 * @brief  Deinitializes the BMP180 sensor.
 * @note   Performs a soft reset of the sensor.
 * @param  None
 * @retval None
 */
void BMP180_DeInit();

/*
 * @brief  Reads the temperature value from the BMP180 sensor.
 * @note   Calculates the actual temperature in Celsius using the uncompensated temperature value.
 * @param  tempValue  Pointer to store the calculated temperature value.
 * @retval e_Status  Status of the operation (STATUS_OK or STATUS_NOT_OK).
 */
e_Status BMP180_ReadTemperature(float *tempValue);

/*
 * @brief  Reads the pressure value from the BMP180 sensor.
 * @note   Calculates the actual pressure in Pa using the uncompensated pressure value and temperature.
 * @param  pressureValue  Pointer to store the calculated pressure value.
 * @retval e_Status  Status of the operation (STATUS_OK or STATUS_NOT_OK).
 */
e_Status BMP180_ReadPressure(int32_t *pressureValue);

/*
 * @brief  Sets the sampling mode for the BMP180 sensor.
 * @note   Updates the sampling mode in the calibration coefficient structure.
 * @param  samplingMode  The desired sampling mode for the sensor.
 * @retval None
 */
void BMP180_SetSamplingMode(e_SamplingMode samplingMode);

/*
 * @brief  Gets the current sampling mode of the BMP180 sensor.
 * @note   Returns the sampling mode from the calibration coefficient structure.
 * @param  None
 * @retval e_SamplingMode  The current sampling mode of the sensor.
 */
e_SamplingMode BMP180_GetSamplingMode();


#endif /* BMP180_H_ */
