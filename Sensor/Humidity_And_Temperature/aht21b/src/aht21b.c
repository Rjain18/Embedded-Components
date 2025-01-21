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
static e_Status AHT21B_ReadRawDataCRC(uint32_t *rawHumidity, uint32_t *rawTemp);

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

	return returnValue;
}

static e_Status AHT21B_ReadRawDataCRC(uint32_t *rawHumidity, uint32_t *rawTemp)
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


e_Status AHT21B_GetTempHumidity()
{
	uint32_t testA, testB;
	AHT21B_ReadRawData(&testA, &testB);

}




