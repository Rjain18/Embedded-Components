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

#define LCD_SERIAL_COM			0x00
#define LCD_I2C_COM				0x01

#define LCD_COMMUNICATION		LCD_I2C_COM   /* Select the Communication with the LCD */

/* Function Definition --------------------------------*/
/*
 * @brief  Checks if the LCD device is ready.
 * @param  deviceAddr  Address of the LCD device.
 * @retval e_Status  Status of the device readiness (STATUS_OK or STATUS_NOT_OK).
 */
e_Status LCD_I2C_IsDeviceReady(uint8_t deviceAddr)
{
    return (HAL_I2C_IsDeviceReady(LCD_I2C_HANDLER, deviceAddr, LCD_TRIAL, LCD_TIMEOUT));
}

/*
 * @brief  Writes data to a specific memory address of the LCD sensor.
 * @param  deviceAddr        Address of the LCD device.
 * @param  memoryAddr        Memory address to write data.
 * @param  writeDataBuffer   Pointer to the data buffer to be written.
 * @param  writeDataSize     Size of the data to be written.
 * @retval e_Status  Status of the write operation (STATUS_OK or STATUS_NOT_OK).
 */
e_Status LCD_I2C_MemoryWrite(uint8_t deviceAddr, uint16_t memoryAddr, uint8_t *writeDataBuffer, uint8_t writeDataSize)
{
    return HAL_I2C_Mem_Write(LCD_I2C_HANDLER, deviceAddr, memoryAddr, LCD_MEMORY_REG_SIZE, writeDataBuffer, (uint16_t)writeDataSize, LCD_TIMEOUT);
}

/*
 * @brief  Reads data from a specific memory address of the LCD sensor.
 * @param  deviceAddr       Address of the LCD device.
 * @param  memoryAddr       Memory address to read data from.
 * @param  readDataBuffer   Pointer to the data buffer to store the read data.
 * @param  readDataSize     Size of the data to be read.
 * @retval e_Status  Status of the read operation (STATUS_OK or STATUS_NOT_OK).
 */
e_Status LCD_I2C_MemoryRead(uint8_t deviceAddr, uint16_t memoryAddr, uint8_t *readDataBuffer, uint8_t readDataSize)
{
    return HAL_I2C_Mem_Read(LCD_I2C_HANDLER, deviceAddr, memoryAddr, LCD_MEMORY_REG_SIZE, readDataBuffer, (uint16_t)readDataSize, LCD_TIMEOUT);
}


#endif /* LCD_CFG_H_ */
