/*
 * bmp180.c
 *
 *  Created on: Dec 17, 2024
 *      Author: jainr
 */

/* Includes -------------------------------------------*/
#include "bmp180.h"
#include "bmp180_cfg.h"

/* Structures -----------------------------------------*/
/* Structure to hold calibration coefficients */
static st_CalibrationCoeff calibrationCoefficient =
{
	0u, /* AC1 */
	0u, /* AC2 */
	0u, /* AC3 */
	0u, /* AC4 */
	0u, /* AC5 */
	0u, /* AC6 */
	0u, /* B1 */
	0u, /* B2 */
	0u, /* MB */
	0u, /* MC */
	0u, /* MD */
	0u, /* B3 */
	0u, /* B4 */
	0u, /* B5 */
	0u, /* B6 */
	0u, /* B7 */
	ULTRA_LOW_POWER, /* Sampling Mode */
};

/* Static Function Declaration ------------------------*/
/**
 * @brief  Performs a soft reset of the BMP180 sensor.
 * @note   Resets the sensor using the soft reset register.
 * @param  None
 * @retval None
 */
static void BMP180_SoftReset();

/**
 * @brief  Reads the calibration coefficients from the BMP180 sensor.
 * @note   Reads configuration data from BMP180 and updates the calibration coefficients.
 * @param  None
 * @retval e_Status  Status of the operation (STATUS_OK or STATUS_NOT_OK).
 */
static e_Status BMP180_ReadCalibrationCoefficient();

/**
 * @brief  Gets the uncompensated temperature value from the BMP180 sensor.
 * @note   Reads the raw temperature data from the sensor.
 * @param  rawTemp  Pointer to store the raw temperature value.
 * @retval e_Status  Status of the operation (STATUS_OK or STATUS_NOT_OK).
 */
static e_Status BMP180_GetUncompensatedTemp(int16_t *rawTemp);

/**
 * @brief  Gets the uncompensated pressure value from the BMP180 sensor.
 * @note   Reads the raw pressure data from the sensor.
 * @param  rawPressure  Pointer to store the raw pressure value.
 * @retval e_Status  Status of the operation (STATUS_OK or STATUS_NOT_OK).
 */
static e_Status BMP180_GetUncompensatedPressure(int16_t *rawPressure);


/* Static Function Definition -------------------------*/
static void BMP180_SoftReset()
{
	uint8_t softReset = BMP180_SOFT_RESET_VALUE;

	/* Write the soft reset command to the soft reset register of the sensor */
	(void)BMP180_MemoryWrite(BMP180_WRITE_ADDRESS, BMP180_SOFT_RESET_REGISTER, &softReset, 0x01u);
}


static e_Status BMP180_ReadCalibrationCoefficient()
{
	e_Status returnStatus = STATUS_NOT_OK;
	uint8_t sensorCalibrationValues[BMP180_CALIBRATION_SIZE] = {0x00u}; /* Array to store calibration values read from the sensor */

	/*Read configuration data from BMP180*/
	returnStatus = BMP180_MemoryRead(BMP180_READ_ADDRESS, BMP180_CALIBRATION_REGISTER, sensorCalibrationValues, BMP180_CALIBRATION_SIZE);

	/* Check if the read operation was successful */
	if(returnStatus == STATUS_OK)
	{
		uint16_t localCalibrationData = 0x0000u;

		/* Iterate through the calibration values */
		for(uint8_t i=0; i<BMP180_CALIBRATION_SIZE; i += 2u)
		{
			/* Convert 8-bit values to 16-bit calibration data */
			localCalibrationData = CONVERT_8BITS_TO_16BITS(sensorCalibrationValues[i], sensorCalibrationValues[i+1]);

			/* Check if the calibration data is valid */
			if( (localCalibrationData != 0x0000u) || (localCalibrationData != 0xFFFFu) )
			{
				/* Update the calibration coefficient structure */
				*(&calibrationCoefficient.AC1 + (i/2)) = localCalibrationData;
			}
			else
			{
				/* Invalid calibration data, update return status */
				returnStatus = STATUS_NOT_OK;
				break;
			}
		}
	}
	else
	{
		/* Handle error case */
	}

	return returnStatus;
}

static e_Status BMP180_GetUncompensatedTemp(int16_t *rawTemp)
{
	e_Status returnValue = STATUS_NOT_OK;
	uint8_t tempStart = BMP180_TEMPERATURE_START;
	uint8_t rawTempArr[2u] = {0x00u}; /* Array to store the raw temperature values read from the sensor */

	/* Check if the rawTemp pointer is not NULL */
	if(rawTemp != NULL)
	{
		/* Write the start command to the control register */
		returnValue = BMP180_MemoryWrite(BMP180_WRITE_ADDRESS, BMP180_CONTROL_REGISTER, &tempStart, 0x01u);

		/* Wait for the measurement to complete */
		COMMON_DELAY(BMP180_WAIT_TIME);

		/* Read the raw temperature values from the sensor */
		returnValue = BMP180_MemoryRead(BMP180_READ_ADDRESS, BMP180_OUT_MSB_REGISTER, rawTempArr, 0x02u);

		/* Convert 8-bit values to 16-bit raw temperature data */
		*rawTemp = CONVERT_8BITS_TO_16BITS(rawTempArr[0u], rawTempArr[1u]);
	}
	else
	{
		/* Handle null pointer */
	}

	return returnValue;
}

static e_Status BMP180_GetUncompensatedPressure(int16_t *rawPressure)
{
	e_Status returnValue = STATUS_NOT_OK;
	uint8_t pressureValue = BMP180_PRESSURE_START + (calibrationCoefficient.samplingMode << 6u);

	/* Calculate the wait time for pressure measurement. Refer Data sheet for explanation */
	uint8_t waitTime = BMP180_WAIT_TIME + (calibrationCoefficient.samplingMode * 4u);
	uint8_t rawPressureArr[3u] = {0x00}; /* Array to store the raw pressure values read from the sensor */

	if(rawPressure != NULL)
	{
		/* Write the start command to the control register */
		returnValue = BMP180_MemoryWrite(BMP180_WRITE_ADDRESS, BMP180_CONTROL_REGISTER, &pressureValue, 0x01u);

		/* Wait for the measurement to complete */
		COMMON_DELAY(waitTime);

		/* Read the raw pressure values from the sensor */
		returnValue = BMP180_MemoryRead(BMP180_READ_ADDRESS, BMP180_OUT_MSB_REGISTER, rawPressureArr, 0x03u);

		/* Convert 8-bit values to 16-bit raw pressure data with sampling mode adjustment */
		*rawPressure = ( (rawPressureArr[0u]<<16u) + (rawPressureArr[1u]<<8u) + rawPressureArr[3u]) >> (8u - calibrationCoefficient.samplingMode);
	}

	else
	{
		/* Handle null pointer */
	}

	return returnValue;
}


/* Function Definition --------------------------------*/
e_Status BMP180_Init()
{
	e_Status returnStatus = STATUS_NOT_OK;

	/* Perform a soft reset of the sensor and wait*/
	BMP180_SoftReset();
	COMMON_DELAY(10);

	/* Check if the sensor is ready */
	returnStatus = BMP180_IsDeviceReady(BMP180_READ_ADDRESS);

	/* If the sensor is ready, read calibration coefficients and set sampling mode */
	if (returnStatus == STATUS_OK)
	{
		returnStatus = BMP180_ReadCalibrationCoefficient();
		BMP180_SetSamplingMode(ULTRA_LOW_POWER);
	}
	else
	{
        /* Handle error case */
	}

	return returnStatus;
}


void BMP180_DeInit()
{
	/* Perform a soft reset of the sensor */
	BMP180_SoftReset();
}


e_Status BMP180_ReadTemperature(float *tempValue)
{
	e_Status returnValue = STATUS_NOT_OK;
	int16_t rawTemp = 0;
	int16_t X1 = 0;
	int16_t X2 = 0;

	/* Get the raw temperature value from the sensor */
	returnValue = BMP180_GetUncompensatedTemp(&rawTemp);

	/* Check if the tempValue pointer is not NULL and the read operation was successful */
	if( (tempValue != NULL) && (returnValue == STATUS_OK) )
	{
		/* Refer data sheet for the temperature algorithm */
		X1 = ( (rawTemp - calibrationCoefficient.AC6) *  calibrationCoefficient.AC5 ) / POWER_OF_2(15) ;
		X2 = (calibrationCoefficient.MC * POWER_OF_2(11)) / (X1 + calibrationCoefficient.MD);
		calibrationCoefficient.B5 = X1 + X2;
		*tempValue = ((calibrationCoefficient.B5 +8) / POWER_OF_2(4)) / 10.0f ;
	}
	else
	{
        /* Handle null pointer or error case */
	}

	return returnValue;
}

e_Status BMP180_ReadPressure(int32_t *pressureValue)
{
	e_Status returnValue = STATUS_NOT_OK;
	float getTemp = 0.0f;
	int16_t rawPressure = 0;
	int16_t X1 = 0;
	int16_t X2 = 0;
	int16_t X3 = 0;
	int32_t p = 0;

	/* Read the temperature value  and raw pressure value from the sensor*/
	returnValue = BMP180_ReadTemperature(&getTemp);
	returnValue = BMP180_GetUncompensatedPressure(&rawPressure);

	/* Check if the pressureValue pointer is not NULL and the read operation was successful */
	if( (pressureValue != NULL) && (returnValue == STATUS_OK))
	{
		/* Refer data sheet for the pressure algorithm */
		calibrationCoefficient.B6 = calibrationCoefficient.B5 - 4000;
		X1 = ( calibrationCoefficient.B2 * (calibrationCoefficient.B6 * calibrationCoefficient.B6/ POWER_OF_2(12)) ) / POWER_OF_2(11);
		X2 = calibrationCoefficient.AC2 * calibrationCoefficient.B6 / POWER_OF_2(11);
		X3 = X1 + X2;
		calibrationCoefficient.B3 = ((((calibrationCoefficient.AC1 * 4) + X3) << calibrationCoefficient.samplingMode) + 2u )/ 4u;
		X1 = calibrationCoefficient.AC3 * calibrationCoefficient.B6 / POWER_OF_2(13);
		X2 = (calibrationCoefficient.B1 * (calibrationCoefficient.B6 * calibrationCoefficient.B6 / POWER_OF_2(12))) / POWER_OF_2(16);
		X3 = ((X1 + X2) + 2) / POWER_OF_2(2);
		calibrationCoefficient.B4 = calibrationCoefficient.AC4 * (uint32_t)(X3 + 32768) / POWER_OF_2(15);
		calibrationCoefficient.B7 = ((uint32_t)rawPressure - calibrationCoefficient.B3) * (50000 >> calibrationCoefficient.samplingMode);

		if(calibrationCoefficient.B7 < 0x80000000)
		{
			p = (calibrationCoefficient.B7 * 2) / calibrationCoefficient.B4;
		}
		else
		{
			p = (calibrationCoefficient.B7 / calibrationCoefficient.B4) * 2;
		}

		X1 = (p/ POWER_OF_2(8)) * (p/ POWER_OF_2(8));
		X1 = (X1 * 3038) / POWER_OF_2(16);
		X2 = (-7357 * p)/ POWER_OF_2(16);
		*pressureValue = p + ( (X1 + X2 + 3791) / POWER_OF_2(4));

	}
	else
	{
		/* Handle null pointer or error case */
	}

	return returnValue;
}

void BMP180_SetSamplingMode(e_SamplingMode samplingMode)
{
	/* Update the sampling mode in the calibration coefficient structure */
	calibrationCoefficient.samplingMode = samplingMode;
}

e_SamplingMode BMP180_GetSamplingMode()
{
	/* Return the current sampling mode from the calibration coefficient structure */
	return calibrationCoefficient.samplingMode;
}




