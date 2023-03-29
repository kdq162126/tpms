/*
 * segment.c
 *
 *  Created on: Mar 2, 2023
 *      Author: khanh
 */

#include "segment.h"
#include "math.h"

 /*       ----A----                        a  b  c  d  e  f  g
  *      |         |                   0_|__|__|__|__|__|__|__|
  *      F         B                   1_|__|__|__|__|__|__|__|
  *      |         |                   2_|__|__|__|__|__|__|__|
  *       ----G----                    3_|__|__|__|__|__|__|__|
  *      |         |                   4_|__|__|__|__|__|__|__|
  *      E         C                   5_|__|__|__|__|__|__|__|
  *      |         |                   6_|__|__|__|__|__|__|__|
  *       ----D----                    .....
  */

#define DIGIT_BLANK                 10
#define DIGIT_DASH                  11

const Digit  _digits[] = {
    {.a = 1, .b = 1, .c = 1, .d = 1, .e = 1, .f = 1, .g = 0},   // 0
    {.a = 0, .b = 1, .c = 1, .d = 0, .e = 0, .f = 0, .g = 0},   // 1
    {.a = 1, .b = 1, .c = 0, .d = 1, .e = 1, .f = 0, .g = 1},   // 2
    {.a = 1, .b = 1, .c = 1, .d = 1, .e = 0, .f = 0, .g = 1},   // 3
    {.a = 0, .b = 1, .c = 1, .d = 0, .e = 0, .f = 1, .g = 1},   // 4
    {.a = 1, .b = 0, .c = 1, .d = 1, .e = 0, .f = 1, .g = 1},   // 5
    {.a = 1, .b = 0, .c = 1, .d = 1, .e = 1, .f = 1, .g = 1},   // 6
    {.a = 1, .b = 1, .c = 1, .d = 0, .e = 0, .f = 0, .g = 0},   // 7
    {.a = 1, .b = 1, .c = 1, .d = 1, .e = 1, .f = 1, .g = 1},   // 8
    {.a = 1, .b = 1, .c = 1, .d = 1, .e = 0, .f = 1, .g = 1},   // 9
    {.a = 0, .b = 0, .c = 0, .d = 0, .e = 0, .f = 0, .g = 0},   // BLANK
    {.a = 0, .b = 0, .c = 0, .d = 0, .e = 0, .f = 0, .g = 1}    // DASH
};

static uint8_t getDigitNumber(int32_t val, uint8_t maxDigitNum);



void SegmentUpdateDigits(Digit* digits, uint32_t val, uint8_t maxDigitNum, uint8_t digitShow) {
    int32_t accumulator = val;
    int32_t digitValue;
    uint8_t realDigitNum = getDigitNumber(val, maxDigitNum);

    for (uint8_t i = 0; i < maxDigitNum; i++) {
        digitValue = accumulator % 10;
        digits[i] = _digits[digitValue];
        if ((digitValue == 0) && (i >= digitShow) && (i >= realDigitNum)) {
            digits[i] = _digits[DIGIT_BLANK];
        }
        accumulator /= 10;
    }
}

void SegmentClearDigits(Digit* digits, uint8_t digitNum) {
    for (uint8_t i = 0; i < digitNum; i++) {
        digits[i] = _digits[DIGIT_BLANK];
    }
}

static uint8_t getDigitNumber(int32_t val, uint8_t maxDigitNum) {
    if (val == 0) return 1;

    int32_t accumulator = 10;
    for (uint8_t i = 0; i < maxDigitNum + 1; i++) {
        accumulator *= 10;
        if (val < accumulator) {
            return i;
        }
    }
    return 1;
}
