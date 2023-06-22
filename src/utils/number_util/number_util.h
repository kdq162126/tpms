/*
 * number_util.h
 *
 *  Created on: May 5, 2023
 *      Author: khanh
 */

#ifndef UTILS_NUMBER_UTIL_NUMBER_UTIL_H_
#define UTILS_NUMBER_UTIL_NUMBER_UTIL_H_

#include "stdint.h"

uint16_t GetUint16(const uint8_t data[]);
uint32_t GetUint32(const uint8_t data[]);
void SetUint16(uint8_t data[], const uint16_t value);
void SetUint32(uint8_t data[], const uint32_t value);

#endif /* UTILS_NUMBER_UTIL_NUMBER_UTIL_H_ */
