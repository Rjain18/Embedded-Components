/*
 * bmp180_cfg.h
 *
 *  Created on: Dec 17, 2024
 *      Author: jainr
 */

#ifndef BMP180_CFG_H_
#define BMP180_CFG_H_

/* Includes -------------------------------------------*/
#include <common.h>
#include "i2c.h"

/* Macro Definition -----------------------------------*/
#define BMP180_I2C_HANDLER			&hi2c1
#define BMP180_TIMEOUT				100u
#define BMP180_TRIAL				3u
#define BMP180_MEMORY_REG_SIZE		I2C_MEMADD_SIZE_8BIT		/* If the memory register size is 8 bits(0x01) or 16 bits */


/* Function Definition --------------------------------*/
/*
 * @brief  Checks if the BMP180 device is ready.
 * @param  deviceAddr  Address of the BMP180 device.
 * @retval e_Status  Status of the device readiness (STATUS_OK or STATUS_NOT_OK).
 */
e_Status BMP180_IsDeviceReady(uint8_t deviceAddr)
{
    return (HAL_I2C_IsDeviceReady(BMP180_I2C_HANDLER, deviceAddr, BMP180_TRIAL, BMP180_TIMEOUT));
}

/*
 * @brief  Writes data to a specific memory address of the BMP180 sensor.
 * @param  deviceAddr        Address of the BMP180 device.
 * @param  memoryAddr        Memory address to write data.
 * @param  writeDataBuffer   Pointer to the data buffer to be written.
 * @param  writeDataSize     Size of the data to be written.
 * @retval e_Status  Status of the write operation (STATUS_OK or STATUS_NOT_OK).
 */
e_Status BMP180_MemoryWrite(uint8_t deviceAddr, uint16_t memoryAddr, uint8_t *writeDataBuffer, uint8_t writeDataSize)
{
    return HAL_I2C_Mem_Write(BMP180_I2C_HANDLER, deviceAddr, memoryAddr, BMP180_MEMORY_REG_SIZE, writeDataBuffer, (uint16_t)writeDataSize, BMP180_TIMEOUT);
}

/*
 * @brief  Reads data from a specific memory address of the BMP180 sensor.
 * @param  deviceAddr       Address of the BMP180 device.
 * @param  memoryAddr       Memory address to read data from.
 * @param  readDataBuffer   Pointer to the data buffer to store the read data.
 * @param  readDataSize     Size of the data to be read.
 * @retval e_Status  Status of the read operation (STATUS_OK or STATUS_NOT_OK).
 */
e_Status BMP180_MemoryRead(uint8_t deviceAddr, uint16_t memoryAddr, uint8_t *readDataBuffer, uint8_t readDataSize)
{
    return HAL_I2C_Mem_Read(BMP180_I2C_HANDLER, deviceAddr, memoryAddr, BMP180_MEMORY_REG_SIZE, readDataBuffer, (uint16_t)readDataSize, BMP180_TIMEOUT);
}


#endif /* BMP180_CFG_H_ */
