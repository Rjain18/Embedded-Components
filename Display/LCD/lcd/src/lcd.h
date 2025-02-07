/**
 * @file LCD.h
 * @brief Driver for LCD communication
 *
 * @date 2025-01-27
 * @author jainr
 */

#ifndef LCD_H_
#define LCD_H_

/* Includes -------------------------------------------*/
#include <common.h>

/* Macro Definition -----------------------------------*/
#define LCD_SEND_DATA			(1 << LCD_RS_MASK)
#define LCD_SEND_CMD			~(1 << LCD_RS_MASK)
#define LCD_READ				(1 << LCD_RW_MASK)
#define LCD_WRITE				~(1 << LCD_RW_MASK)
#define LCD_ENABLE_HIGH			(1 << LCD_EN_MASK)
#define LCD_ENABLE_LOW			~(1 << LCD_EN_MASK)
#define LCD_BACKLIGHT_ON		(1 << LCD_BCKL_MASK)
#define LCD_BACKLIGHT_OFF		~(1 << LCD_BCKL_MASK)

#define LCD_RS_MASK				0u
#define LCD_RW_MASK				1u
#define LCD_EN_MASK				2u
#define LCD_BCKL_MASK			3u

#define LCD_PACKET_SZ			4u

/* Instructions */
#define LCD_CLEAR_DISPLAY		0x01
#define LCD_RET_HOME			0x02
#define LCD_ENTRY_MODE_SET		0x04
#define LCD_DISPLAY_CONTROL		0x08
#define LCD_DISP_CURSOR_SHIFT	0x10
#define LCD_FUNC_SET			0x20
#define LCD_SET_CGRAM_ADDR		0x40
#define LCD_SET_DDRAM_ADDR		0x80

/* Data Length (DL) */
#define LCD_8BITMODE 			0x10
#define LCD_4BITMODE 			0x00

/* Number of display line (N) */
#define LCD_2LINE 				0x08
#define LCD_1LINE 				0x00

/* Character font (F) */
#define LCD_5x10DOTS 			0x04
#define LCD_5x8DOTS 			0x00

/* Display On/Off */
#define LCD_DISPLAY_ON 			0x04
#define LCD_DISPLAY_OFF 		0x00

/* Cursor On/off */
#define LCD_CURSOR_ON			0x02
#define LCD_CURSOR_OFF 			0x00

/* Blinking On/Off */
#define LCD_BLINK_ON			0x01
#define LCD_BLINK_OFF 			0x00

/* Shifting On/off */
#define LCD_SHIFT_ON			0x01
#define LCD_SHIFT_OFF			0x00

/* Increment & Decrement (I/D) control*/
#define LCD_INCREMENT			0x02
#define LCD_DECREMENT			0x00

/* Display shift or Cursor move */
#define LCD_DISPLAY_SHIFT		0x08
#define	LCD_CURSOR_MOVE			0x00

/* Shift to Right/Left */
#define LCD_SHIFT_RIGHT			0x40
#define LCD_SHIFT_LEFT			0x00

/* Row number */
#define LCD_ROW_0				0x00
#define LCD_ROW_1				0x40


/* Enums ----------------------------------------------*/

/* Structures -----------------------------------------*/

/* Variables ------------------------------------------*/

/* Function Declaration -------------------------------*/

/**
 * @brief  Initializes the LCD.
 *
 * This function initializes the LCD by checking if the device is ready,
 * and then sending the necessary initialization commands.
 *
 * @return e_Status Returns the status of the initialization.
 */
e_Status LCD_Init();

 /**
  * @brief  Sets the cursor position on the LCD.
  *
  * This function sets the cursor position on the LCD to the specified row and column.
  *
  * @param  rowPos The row position (0- LCD_ROW_NO).
  * @param  colPos The column position (0- LCD_CHAR_NO).
  * @return e_Status Returns the status of the cursor position update.
  */
e_Status LCD_SetCursor(uint8_t rowPos, uint8_t colPos);

/**
 * @brief  Sends a string to the LCD.
 *
 * This function sends a string of data to the LCD.
 *
 * @param  stringData Pointer to the string data to be sent.
 * @param  dataSize The size of the data to be sent.
 * @return e_Status Returns the status of the transmission.
 */
e_Status LCD_SendString(char* stringData, uint8_t dataSize);

/**
 * @brief  Turns on the LCD display.
 *
 * This function sets the display control flag for turning on the display
 * and sends the updated display control byte to the LCD.
 *
 * @return e_Status Returns the status of the transmission.
 */
e_Status LCD_DisplayOn();

/**
 * @brief  Turns off the LCD display.
 *
 * This function clears the display control flag for turning off the display
 * and sends the updated display control byte to the LCD.
 *
 * @return e_Status Returns the status of the transmission.
 */
e_Status LCD_DisplayOff();

/**
 * @brief  Turns on the cursor.
 *
 * This function sets the display control flag for turning on the cursor
 * and sends the updated display control byte to the LCD.
 *
 * @return e_Status Returns the status of the transmission.
 */
e_Status LCD_CursorOn();

/**
 * @brief  Turns off the cursor.
 *
 * This function clears the display control flag for turning off the cursor
 * and sends the updated display control byte to the LCD.
 *
 * @return e_Status Returns the status of the transmission.
 */
e_Status LCD_CursorOff();

/**
 * @brief  Turns on the blinking cursor.
 *
 * This function sets the display control flag for turning on the blinking cursor
 * and sends the updated display control byte to the LCD.
 *
 * @return e_Status Returns the status of the transmission.
 */
e_Status LCD_BlinkOn();

/**
 * @brief  Turns off the blinking cursor.
 *
 * This function clears the display control flag for turning off the blinking cursor
 * and sends the updated display control byte to the LCD.
 *
 * @return e_Status Returns the status of the transmission.
 */
e_Status LCD_BlinkOff();

/**
 * @brief  Clears the LCD display.
 *
 * This function sends the clear display command to the LCD
 * and introduces a delay for the command to take effect.
 *
 * @return e_Status Returns the status of the transmission.
 */
e_Status LCD_ClearDisplay();



#endif /* LCD_H_ */
