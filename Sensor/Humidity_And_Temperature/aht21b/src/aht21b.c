/*
 * aht21b.c
 *
 *  Created on: Jan 18, 2025
 *      Author: jainr
 */


/* Includes -------------------------------------------*/
#include "aht21b.h"
#include "aht21b_cfg.h"

/* Variables ------------------------------------------*/


/* Static Function Declaration ------------------------*/
static e_Status AHT21B_ResetRegisters();

static e_Status AHT21B_ReadRawData(uint32_t *rawHumidity, uint32_t *rawTemp);

static e_Status AHT21B_CheckCRC(uint8_t crcData);

/* Static Function Definition -------------------------*/

static e_Status AHT21B_ResetRegisters()
{
	e_Status returnValue = STATUS_NOT_OK;
	uint8_t readByte[3u] = {0x00, 0x00, 0x00};
	uint8_t resetValues[2u] = {0x00, 0x00};
	uint8_t	loopVal = 0u;

	/* Array to store in all the reset registers for "for" loop */
	uint8_t registersArray[AHT21B_REGISTER_ARRAY_SIZE] = {AHT21B_REGISTER_A_ADDRESS, AHT21B_REGISTER_B_ADDRESS, AHT21B_REGISTER_C_ADDRESS};

	for(loopVal = 0; loopVal < AHT21B_REGISTER_ARRAY_SIZE; loopVal++)
	{
		/*Send 0x00, 0x00 to the reset registers*/
		returnValue = AHT21B_MemoryWrite(AHT21B_I2C_WRITE_ADDRESS, registersArray[loopVal], resetValues, 2u);

		/* Read the Status byte*/
		COMMON_DELAY(5);
		returnValue = AHT21B_MemoryRead(AHT21B_I2C_READ_ADDRESS, AHT21B_STATUS_ADDRESS, readByte, 3u);

		if(returnValue == STATUS_OK)
		{
			/* Write the calibrated data in the register */
			COMMON_DELAY(10);
			returnValue = AHT21B_MemoryWrite(AHT21B_I2C_WRITE_ADDRESS, (0xB0 | registersArray[loopVal]), &readByte[1u], 2u);

			/* Reseting the Second and third status byte */
			readByte[1u] = 0x00;
			readByte[2u] = 0x00;
			COMMON_DELAY(1);
		}
		else
		{
			break;
		}
	}
	return returnValue;
}

static e_Status AHT21B_ReadRawData(uint32_t *rawHumidity, uint32_t *rawTemp)
{
	e_Status returnValue = STATUS_NOT_OK;
	uint8_t rawDataBuffer[7u] = {0x00};
	uint8_t startMeasureRequest[AHT21B_MEASUREMENT_SIZE] = {AHT21B_MEASUREMENT_BYTE0, AHT21B_MEASUREMENT_BYTE1};
	uint8_t count = 0;
	uint32_t localBuffer = 0x00;

	/* Trigger measurement and wait for 80ms as per datahsheet*/
	returnValue = AHT21B_MemoryWrite(AHT21B_I2C_WRITE_ADDRESS, AHT21B_START_MEASUREMENT, startMeasureRequest, AHT21B_MEASUREMENT_SIZE);
	COMMON_DELAY(80);

	if(returnValue == STATUS_OK)
	{
		/* Wait for Status byte to indicate the completion of measurement */
		do{
			(void)AHT21B_MemoryRead(AHT21B_I2C_READ_ADDRESS, AHT21B_STATUS_ADDRESS, rawDataBuffer, AHT21B_STATUS_SIZE);
			count++;
			if(count >= AHT21B_MEASUREMENT_TIMEOUT)
			{
				break;
			}
		}while( (rawDataBuffer[0u] & AHT21B_STATUS_BUSY) == AHT21B_STATUS_BUSY );

		AHT21B_MemoryRead(AHT21B_I2C_READ_ADDRESS, AHT21B_STATUS_ADDRESS, rawDataBuffer, 7u);

	}
	else
	{
		/*Error handler*/
	}

#if(AHT21B_DATA_CRC_CHECK == 1u)

#endif
	/* Store the 20-bits humidity data */
	localBuffer = (localBuffer | rawDataBuffer[1u] ) << 8u;
	localBuffer = (localBuffer | rawDataBuffer[2u] ) << 8u;
	localBuffer = (localBuffer | rawDataBuffer[3u] );
	localBuffer = localBuffer >> 4u;
	*rawHumidity = localBuffer;

	/* Store the 20-bits temperature data */
	localBuffer = 0u;
	localBuffer = (localBuffer | rawDataBuffer[3u] ) << 8u;
	localBuffer = (localBuffer | rawDataBuffer[4u] ) << 8u;
	localBuffer = (localBuffer | rawDataBuffer[5u] );
	localBuffer = localBuffer & 0xFFFFF;
	*rawTemp = localBuffer;



	return returnValue;
}

static e_Status AHT21B_CheckCRC(uint8_t crcData)
{

}

/* Function Definition --------------------------------*/

e_Status AHT21B_Init()
{
	e_Status returnValue = STATUS_NOT_OK;
	uint8_t statusWord = 0u;

	returnValue = AHT21B_MemoryRead(AHT21B_I2C_READ_ADDRESS, AHT21B_STATUS_ADDRESS, &statusWord, AHT21B_STATUS_SIZE);

	/* 	Check if the read operation was successful */
	if(returnValue == STATUS_OK)
	{
		/* Check if the Status word is OK. If not then Initialize 0x1B, 0x1C, 0x1E registers */
		if( (statusWord & AHT21B_STATUS_CONST) != AHT21B_STATUS_CONST )
		{
			AHT21B_ResetRegisters();
		}
		else
		{
			/* Do nothing */
		}
	}
	else
	{
		/* Error Handling */
	}

	/* According to Datasheet, wait 10 ms before sending Start measurement command. This can be removed if needed */
	COMMON_DELAY(10);
	return returnValue;
}


e_Status AHT21B_GetTempHumidity(float *humidityVal, float *tempVal)
{
	e_Status returnValue = STATUS_NOT_OK;
	uint32_t rawHumidity = 0;
	uint32_t rawTemp = 0;
	float localData = 0.0f;

	/* Read the raw humidity and temperature */
	returnValue = AHT21B_ReadRawData(&rawHumidity, &rawTemp);

	/* Calculate the Relative humidity and Temperature if the reading is successful */
	if(returnValue == STATUS_OK)
	{
		/* Calculate the Relative Humidity in % */
		/* Note: 2^20 = 1048576 */
		localData = ((float)rawHumidity/1048576.0) * 100.0f;
		*humidityVal = localData;

		/* Calculate the Temperature in Celcius */
		localData = (((float)rawTemp / 1048576.0) * 200.0f) - 50.0f;
		*tempVal = localData;
	}

	return returnValue;

}




