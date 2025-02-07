/**
 * @file common.h
 * @brief Contains common objects used in the project
 *
 * This file contains the implementation for AHT21B sensor interfacing functions.
 *
 * @date 2025-01-18
 * @author jainr
 */

#ifndef COMMON_H_
#define COMMON_H_

#include <stdint.h>
#include <stm32f0xx_hal.h>

#define COMMON_DELAY(x)						( HAL_Delay(x) ) /* For STM32 */
#define CONVERT_8BITS_TO_16BITS(x,y)		( (x << 8) | (y) )

#define POWER_OF_2(x)						( 1 << x )


typedef enum e_Status
{
	STATUS_OK = 0x00,
	STATUS_NOT_OK = 0x01,
	STATUS_BUSY = 0x02,
	STATUS_TIMEOUT = 0x03,
	STATUS_CRC_ERROR = 0x04
} e_Status;



#endif /* COMMON_H_ */
