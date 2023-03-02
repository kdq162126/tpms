/*
 * segment.h
 *
 *  Created on: Mar 2, 2023
 *      Author: khanh
 */

#ifndef SERVICE_SEGMENT_SEGMENT_H_
#define SERVICE_SEGMENT_SEGMENT_H_

#include "stdint.h"

typedef struct Bit_t Bit;
struct Bit_t {
    uint8_t b : 1;
};

typedef struct Digit_t Digit;
struct Digit_t {
    uint8_t a : 1;
    uint8_t b : 1;
    uint8_t c : 1;
    uint8_t d : 1;
    uint8_t e : 1;
    uint8_t f : 1;
    uint8_t g : 1;
};

typedef union Byte_t Byte;
union Byte_t {
    uint8_t val;
    Bit b[8];
};

void SegmentUpdateDigits(Digit* digits, uint32_t val, uint8_t maxDigitNum, uint8_t digitShow);
void SegmentClearDigits(Digit* digits, uint8_t digitNum);


#endif /* SERVICE_SEGMENT_SEGMENT_H_ */
