/**
 * @file LCD.c
 * @brief Driver for LCD communication
 *
 * @date 2025-01-27
 * @author jainr
 */


/* Includes -------------------------------------------*/
#include "lcd.h"
#include "lcd_cfg.h"

/* Variables ------------------------------------------*/
static uint8_t displayControl = 0x00; /* Used for storing the display information */

/* Static Function Declaration ------------------------*/

/**
 * @brief  Sends a command to the LCD.
 *
 * This function sends a command byte to the LCD.
 * It divides the byte into two nibbles and transmits
 * each nibble sequentially.
 *
 * @param  cmd The command byte to be sent.
 * @return e_Status Returns the status of the transmission.
 */
e_Status LCD_CommandWrite(uint8_t cmd);

/**
 * @brief  Sends data to the LCD.
 *
 * This function sends a data byte to the LCD.
 * It divides the byte into two nibbles and transmits
 * each nibble sequentially.
 *
 * @param  data The data byte to be sent.
 * @return e_Status Returns the status of the transmission.
 */
e_Status LCD_DataWrite(uint8_t data);


/* Static Function Definition -------------------------*/

e_Status LCD_CommandWrite(uint8_t cmd)
{
	e_Status returnValue = STATUS_NOT_OK;
	uint8_t sendPacket[LCD_PACKET_SZ] = {0x00};
    uint8_t dataMSB = 0u;  /* Most significant nibble of command */
    uint8_t dataLSB = 0u;  /* Least significant nibble of command */

    dataMSB = cmd & 0xF0;  /* Extract the most significant nibble */
    dataLSB = (cmd << 4u) & 0xF0;  /* Extract the least significant nibble */

    /* Set backlight and enable high for MSB data packet */
	dataMSB |= LCD_BACKLIGHT_ON | LCD_ENABLE_HIGH;
	sendPacket[0u] = dataMSB;

	/* Set enable low */
	dataMSB &= LCD_ENABLE_LOW;
	sendPacket[1u] = dataMSB;

    /* Set backlight and enable high for LSB data packet */
	dataLSB |= LCD_BACKLIGHT_ON | LCD_ENABLE_HIGH;
	sendPacket[2u] = dataLSB;

	/* Set enable low */
	dataLSB &= LCD_ENABLE_LOW;
	sendPacket[3u] = dataLSB;

	returnValue = LCD_Transmit(sendPacket, LCD_PACKET_SZ);

	return returnValue;
}

e_Status LCD_DataWrite(uint8_t data)
{
	e_Status returnValue = STATUS_NOT_OK;
	uint8_t sendPacket[LCD_PACKET_SZ] = {0x00};
    uint8_t dataMSB = 0u;  /* Most significant nibble of data */
    uint8_t dataLSB = 0u;  /* Least significant nibble of data */

    dataMSB = data & 0xF0;  /* Extract the most significant nibble */
    dataLSB = (data << 4u) & 0xF0;  /* Extract the least significant nibble */

    /* Set backlight, send data, and enable high for MSB data packet*/
	dataMSB |= LCD_BACKLIGHT_ON | LCD_SEND_DATA | LCD_ENABLE_HIGH;
	sendPacket[0u] = dataMSB;

	/* Set enable low */
	dataMSB &= LCD_ENABLE_LOW;
	sendPacket[1u] = dataMSB;

    /* Set backlight, send data, and enable high for LSB data packet*/
	dataLSB |= LCD_BACKLIGHT_ON | LCD_SEND_DATA | LCD_ENABLE_HIGH;
	sendPacket[2u] = dataLSB;

	/* Set enable low */
	dataLSB &= LCD_ENABLE_LOW;
	sendPacket[3u] = dataLSB;

	returnValue = LCD_Transmit(sendPacket, LCD_PACKET_SZ);

	return returnValue;
}

/* Function Definition --------------------------------*/

e_Status LCD_Init()
{
	e_Status returnValue = STATUS_NOT_OK;

	/* Check if the device is ready */
	returnValue = LCD_IsDeviceReady();

	if(returnValue == STATUS_OK)
	{
		COMMON_DELAY(45);
		returnValue = LCD_CommandWrite(LCD_FUNC_SET | LCD_8BITMODE); /* Function set: 8-bit mode */
		COMMON_DELAY(5);
		returnValue = LCD_CommandWrite(LCD_FUNC_SET | LCD_8BITMODE); /* Function set: 8-bit mode */
		COMMON_DELAY(1);
		returnValue = LCD_CommandWrite(LCD_FUNC_SET | LCD_8BITMODE); /* Function set: 8-bit mode */
		COMMON_DELAY(10);
		returnValue = LCD_CommandWrite(LCD_FUNC_SET | LCD_4BITMODE); /* Function set: 4-bit mode */
		COMMON_DELAY(10);
		returnValue = LCD_CommandWrite(LCD_FUNC_SET | LCD_4BITMODE | LCD_2LINE | LCD_5x8DOTS); /* Function set: 4-bit mode, 2 lines, 5x8 dots */
		COMMON_DELAY(10);
		displayControl = LCD_DISPLAY_CONTROL | LCD_DISPLAY_OFF | LCD_CURSOR_OFF | LCD_BLINK_OFF; /* Set display control: display off, cursor off, blink off */
		returnValue = LCD_CommandWrite(displayControl);
		COMMON_DELAY(1);
		returnValue = LCD_CommandWrite(LCD_CLEAR_DISPLAY); /* Clear display */
		COMMON_DELAY(5);
		returnValue = LCD_CommandWrite(LCD_ENTRY_MODE_SET | LCD_INCREMENT); /* Set entry mode: increment */
		COMMON_DELAY(1);
		displayControl = LCD_DISPLAY_CONTROL | LCD_DISPLAY_ON | LCD_CURSOR_ON | LCD_BLINK_OFF; /* Set display control: display on, cursor on, blink off */
		returnValue = LCD_CommandWrite(displayControl);
		COMMON_DELAY(1);
	}
	else
	{

	}

	return returnValue;
}


e_Status LCD_SetCursor(uint8_t rowPos, uint8_t colPos)
{
	e_Status returnValue = STATUS_NOT_OK;
	uint8_t cursorPos = 0x00;

	/* Check if the row and column input are withine range */
	if(rowPos < LCD_ROW_NO && colPos < LCD_CHAR_NO)
	{
		switch(rowPos)
		{
			case 0u: /* Set cursor position for row 0 */
				cursorPos = colPos | LCD_SET_DDRAM_ADDR |  LCD_ROW_0;
				returnValue = STATUS_OK;
				break;

			case 1u: /* Set cursor position for row 1 */
				cursorPos = colPos | LCD_SET_DDRAM_ADDR |  LCD_ROW_1;
				returnValue = STATUS_OK;
				break;

			default: //Code should not come here
				returnValue = STATUS_NOT_OK;
				break;
		}
	}
	else
	{
		/* Error Handling */
	}

	if(returnValue == STATUS_OK)
	{
		returnValue = LCD_CommandWrite(cursorPos);
	}
	else
	{

	}

	return returnValue;
}

e_Status LCD_SendString(char* stringData, uint8_t dataSize)
{
	e_Status returnValue = STATUS_NOT_OK;

	/* Checking is data is present in stringData */
	if( (stringData != NULL) && (dataSize != 0u) )
	{
		for(uint8_t i =0; i < dataSize; i++)
		{
			LCD_DataWrite((uint8_t)( *(stringData + i)));
		}
	}
	else
	{
		/* Error Handling */
	}

	return returnValue;

}

e_Status LCD_DisplayOn()
{
	e_Status returnValue = STATUS_NOT_OK;
	displayControl |= LCD_DISPLAY_ON;

	returnValue = LCD_CommandWrite(displayControl);

	return returnValue;
}

e_Status LCD_DisplayOff()
{
	e_Status returnValue = STATUS_NOT_OK;
	displayControl &= ~LCD_DISPLAY_ON;

	returnValue = LCD_CommandWrite(displayControl);

	return returnValue;
}

e_Status LCD_CursorOn()
{
	e_Status returnValue = STATUS_NOT_OK;
	displayControl |= LCD_CURSOR_ON;

	returnValue = LCD_CommandWrite(displayControl);

	return returnValue;
}

e_Status LCD_CursorOff()
{
	e_Status returnValue = STATUS_NOT_OK;
	displayControl &= ~LCD_CURSOR_ON;

	returnValue = LCD_CommandWrite(displayControl);

	return returnValue;
}

e_Status LCD_BlinkOn()
{
	e_Status returnValue = STATUS_NOT_OK;
	displayControl |= LCD_BLINK_ON;

	returnValue = LCD_CommandWrite(displayControl);

	return returnValue;
}

e_Status LCD_BlinkOff()
{
	e_Status returnValue = STATUS_NOT_OK;
	displayControl &= ~LCD_BLINK_ON;

	returnValue = LCD_CommandWrite(displayControl);

	return returnValue;
}

e_Status LCD_ClearDisplay()
{
	e_Status returnValue = STATUS_NOT_OK;

	returnValue = LCD_CommandWrite(LCD_CLEAR_DISPLAY);
	COMMON_DELAY(2);

	return returnValue;
}
