/**
 * @file lcd_cfg.h
 * @brief Configuration for LCD
 *
 * @date 2025-01-27
 * @author jainr
 */

#ifndef LCD_CFG_H_
#define LCD_CFG_H_

/* Includes -------------------------------------------*/
#include <common.h>
#include "i2c.h"

/* Macro Definition -----------------------------------*/
#define LCD_I2C_HANDLER			&hi2c1
#define LCD_TIMEOUT				100u
#define LCD_TRIAL				3u
#define LCD_MEMORY_REG_SIZE		I2C_MEMADD_SIZE_8BIT		/* If the memory register size is 8 bits(0x01) or 16 bits */
#define LCD_I2C_ADDRESS			0x4E

#define LCD_SERIAL_COM			0x00
#define LCD_I2C_COM				0x01

/* Select the Communication with the LCD */
#define LCD_COMMUNICATION		LCD_I2C_COM

/* Selection of LCD type */
#define LCD_CHAR_NO				16u
#define LCD_ROW_NO				2u

/* Function Definition --------------------------------*/

#if(LCD_COMMUNICATION == LCD_I2C_COM)
/*
 * @brief  Checks if the LCD device is ready.
 * @param  deviceAddr  Address of the LCD device.
 * @retval e_Status  Status of the device readiness (STATUS_OK or STATUS_NOT_OK).
 */
e_Status LCD_IsDeviceReady()
{
    return (HAL_I2C_IsDeviceReady(LCD_I2C_HANDLER, LCD_I2C_ADDRESS, LCD_TRIAL, LCD_TIMEOUT));
}


e_Status LCD_Transmit(uint8_t *writeDataBuffer, uint8_t writeDataSize)
{
    return HAL_I2C_Master_Transmit(LCD_I2C_HANDLER, LCD_I2C_ADDRESS, writeDataBuffer, (uint16_t)writeDataSize, LCD_TIMEOUT);

}
#endif /*(LCD_COMMUNICATION == LCD_I2C_COM)*/

#endif /* LCD_CFG_H_ */
