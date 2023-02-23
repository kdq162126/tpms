/*
 * segment_driver.h
 *
 *  Created on: Feb 22, 2023
 *      Author: khanh
 */

#ifndef COMPONENT_SEGMENT_DRIVER_SEGMENT_DRIVER_H_
#define COMPONENT_SEGMENT_DRIVER_SEGMENT_DRIVER_H_

#include "stdint.h"

typedef struct Bit_t Bit;
struct Bit_t {
    uint8_t bit : 1;
    void (*set)(uint8_t);
};

typedef struct Digit_t Digit;
struct Digit_t {
    Bit bit[7];
};

#endif /* COMPONENT_SEGMENT_DRIVER_SEGMENT_DRIVER_H_ */
