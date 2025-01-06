/*
 * bmp180.c
 *
 *  Created on: Dec 17, 2024
 *      Author: jainr
 */

#include "bmp180.h"
#include "bmp180_cfg.h"

static st_CalibrationCoeff calibrationCoefficient =
{
	0u,
	0u,
	0u,
	0u,
	0u,
	0u,
	0u,
	0u,
	0u,
	0u,
	0u,
	0u,
	0u,
	0u,
	0u,
	0u,
	ULTRA_LOW_POWER,
};

static void BMP180_SoftReset();
static e_Status BMP180_ReadCalibrationCoefficient();
static e_Status BMP180_GetUncompensatedTemp(int16_t *rawTemp);
static e_Status BMP180_GetUncompensatedPressure(int16_t *rawPressure);



static void BMP180_SoftReset()
{
	uint8_t softReset = BMP180_SOFT_RESET_VALUE;
	(void)BMP180_MemoryWrite(BMP180_WRITE_ADDRESS, BMP180_SOFT_RESET_REGISTER, &softReset, 0x01u);
}


static e_Status BMP180_ReadCalibrationCoefficient()
{
	e_Status returnStatus = STATUS_NOT_OK;
	uint8_t sensorCalibrationValues[BMP180_CALIBRATION_SIZE] = {0x00u};

	/*Read configuration data from BMP180*/
	returnStatus = BMP180_MemoryRead(BMP180_READ_ADDRESS, BMP180_CALIBRATION_REGISTER, sensorCalibrationValues, BMP180_CALIBRATION_SIZE);

	if(returnStatus == STATUS_OK)
	{
		uint16_t localCalibrationData = 0x0000u;
		for(uint8_t i=0; i<BMP180_CALIBRATION_SIZE; i += 2u)
		{
			localCalibrationData = CONVERT_8BITS_TO_16BITS(sensorCalibrationValues[i], sensorCalibrationValues[i+1]);
			if( (localCalibrationData != 0x0000u) || (localCalibrationData != 0xFFFFu) )
			{
				*(&calibrationCoefficient.AC1 + (i/2)) = localCalibrationData;
			}
			else
			{
				returnStatus = STATUS_NOT_OK;
				break;
			}
		}
	}
	else
	{

	}

	return returnStatus;
}


static e_Status BMP180_GetUncompensatedTemp(int16_t *rawTemp)
{
	e_Status returnValue = STATUS_NOT_OK;
	uint8_t tempStart = BMP180_TEMPERATURE_START;
	uint8_t rawTempArr[2u] = {0x00u};

	if(rawTemp != NULL)
	{
		returnValue = BMP180_MemoryWrite(BMP180_WRITE_ADDRESS, BMP180_CONTROL_REGISTER, &tempStart, 0x01u);
		COMMON_DELAY(BMP180_WAIT_TIME);
		returnValue = BMP180_MemoryRead(BMP180_READ_ADDRESS, BMP180_OUT_MSB_REGISTER, rawTempArr, 0x02u);
		*rawTemp = CONVERT_8BITS_TO_16BITS(rawTempArr[0u], rawTempArr[1u]);
	}
	else
	{

	}

	return returnValue;
}

static e_Status BMP180_GetUncompensatedPressure(int16_t *rawPressure)
{
	e_Status returnValue = STATUS_NOT_OK;
	uint8_t pressureValue = BMP180_PRESSURE_START + (calibrationCoefficient.samplingMode << 6u);
	uint8_t waitTime = BMP180_WAIT_TIME + (calibrationCoefficient.samplingMode * 4u);
	uint8_t rawPressureArr[3u] = {0x00};

	if(rawPressure != NULL)
	{
		returnValue = BMP180_MemoryWrite(BMP180_WRITE_ADDRESS, BMP180_CONTROL_REGISTER, &pressureValue, 0x01u);
		COMMON_DELAY(waitTime);
		returnValue = BMP180_MemoryRead(BMP180_READ_ADDRESS, BMP180_OUT_MSB_REGISTER, rawPressureArr, 0x03u);
		*rawPressure = ( (rawPressureArr[0u]<<16u) + (rawPressureArr[1u]<<8u) + rawPressureArr[3u]) >> (8u - calibrationCoefficient.samplingMode);
	}

	else
	{

	}

	return returnValue;
}


e_Status BMP180_Init()
{
	e_Status returnStatus = STATUS_NOT_OK;

	BMP180_SoftReset();
	COMMON_DELAY(10);

	returnStatus = BMP180_IsDeviceReady(BMP180_READ_ADDRESS);

	if (returnStatus == STATUS_OK)
	{
		returnStatus = BMP180_ReadCalibrationCoefficient();
		BMP180_SetSamplingMode(ULTRA_LOW_POWER);
	}
	else
	{

	}

	return returnStatus;
}


void BMP180_DeInit()
{
	BMP180_SoftReset();
}


e_Status BMP180_ReadTemperature(float *tempValue)
{
	e_Status returnValue = STATUS_NOT_OK;
	int16_t rawTemp = 0;
	int16_t X1 = 0;
	int16_t X2 = 0;

	returnValue = BMP180_GetUncompensatedTemp(&rawTemp);

	if( (tempValue != NULL) && (returnValue == STATUS_OK) )
	{
		X1 = ( (rawTemp - calibrationCoefficient.AC6) *  calibrationCoefficient.AC5 ) / POWER_OF_2(15) ;
		X2 = (calibrationCoefficient.MC * POWER_OF_2(11)) / (X1 + calibrationCoefficient.MD);
		calibrationCoefficient.B5 = X1 + X2;
		*tempValue = ((calibrationCoefficient.B5 +8) / POWER_OF_2(4)) / 10.0f ;
	}
	else
	{

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

	returnValue = BMP180_ReadTemperature(&getTemp);
	returnValue = BMP180_GetUncompensatedPressure(&rawPressure);

	if( (pressureValue != NULL) && (returnValue == STATUS_OK))
	{
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

	}

	return returnValue;
}

void BMP180_SetSamplingMode(e_SamplingMode samplingMode)
{
	calibrationCoefficient.samplingMode = samplingMode;
}

e_SamplingMode BMP180_GetSamplingMode()
{
	return calibrationCoefficient.samplingMode;
}




