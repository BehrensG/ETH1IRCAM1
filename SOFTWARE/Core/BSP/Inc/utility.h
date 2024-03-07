/*
 * utility.h
 *
 *  Created on: Feb 15, 2024
 *      Author: grzegorz
 */

#ifndef BSP_INC_UTILITY_H_
#define BSP_INC_UTILITY_H_

#include "main.h"
#include "BSP.h"

bool UTIL_Timeout(uint32_t start, uint32_t timeout);
uint32_t UTIL_RandomNumbersGeneration(void);
uint32_t UTIL_CalculateCRC(uint8_t* data_buffer, uint32_t buffer_size);

#endif /* BSP_INC_UTILITY_H_ */
