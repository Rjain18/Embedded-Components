/*
 * bmp180_cfg.h
 *
 *  Created on: Dec 17, 2024
 *      Author: jainr
 */

#ifndef BMP180_CFG_H_
#define BMP180_CFG_H_

#include <common.h>
#include "i2c.h"

#define BMP180_I2C_HANDLER			&hi2c1
#define BMP180_TIMEOUT				100u
#define BMP180_TRIAL				3u
#define BMP180_MEMORY_REG_SIZE		I2C_MEMADD_SIZE_8BIT		/* If the memory register size is 8 bits(0x01) or 16 bits */


e_Status BMP180_IsDeviceReady(uint8_t deviceAddr)
{
	return (HAL_I2C_IsDeviceReady(BMP180_I2C_HANDLER, deviceAddr, BMP180_TRIAL, BMP180_TIMEOUT ));
}

e_Status BMP180_MemoryWrite(uint8_t deviceAddr, uint16_t memoryAddr, uint8_t *writeDataBuffer, uint8_t writeDataSize)
{
	return HAL_I2C_Mem_Write(BMP180_I2C_HANDLER, deviceAddr, memoryAddr, BMP180_MEMORY_REG_SIZE, writeDataBuffer, (uint16_t)writeDataSize, BMP180_TIMEOUT );
}

e_Status BMP180_MemoryRead(uint8_t deviceAddr, uint16_t memoryAddr, uint8_t *readDataBuffer, uint8_t readDataSize)
{
	return HAL_I2C_Mem_Read(BMP180_I2C_HANDLER, deviceAddr, memoryAddr, BMP180_MEMORY_REG_SIZE, readDataBuffer, (uint16_t)readDataSize, BMP180_TIMEOUT );
}





#endif /* BMP180_CFG_H_ */
