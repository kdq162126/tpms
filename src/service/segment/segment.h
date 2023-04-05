/*
 * segment.h
 *
 *  Created on: Mar 2, 2023
 *      Author: khanh
 */

#ifndef SERVICE_SEGMENT_SEGMENT_H_
#define SERVICE_SEGMENT_SEGMENT_H_

#include "stdint.h"
#include "stdbool.h"

typedef struct bits_t bits;
struct bits_t {
    uint8_t    b0 : 1;
    uint8_t    b1 : 1;
    uint8_t    b2 : 1;
    uint8_t    b3 : 1;
    uint8_t    b4 : 1;
    uint8_t    b5 : 1;
    uint8_t    b6 : 1;
    uint8_t    b7 : 1;
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
    bits bits;
};

typedef struct SegElement_t SegElement;
struct SegElement_t {
    int32_t value;
    bool    isBlink;
    void 	(*clear)(SegElement*);
    void    (*update)(SegElement*, const int32_t);
    void    (*write)(SegElement*, const int32_t);
};

void SegmentUpdateDigits(Digit* digits, uint32_t val, uint8_t maxDigitNum, uint8_t digitShow);
void SegmentClearDigits(Digit* digits, uint8_t digitNum);

static inline void SegmentUpdate(SegElement* seg, const int32_t newVal) {
    seg->update(seg, newVal);
}

static inline void SegmentWrite(SegElement* seg, const int32_t newVal) {
    seg->write(seg, newVal);
}


#endif /* SERVICE_SEGMENT_SEGMENT_H_ */
