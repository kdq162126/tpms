/*
 * number_util.c
 *
 *  Created on: May 5, 2023
 *      Author: khanh
 */

#include "number_util.h"
#include "stdint.h"

typedef union {
    uint8_t  u8[8];  /**< 8 bytes */
    uint16_t u16[4]; /**< 4 words */
    uint32_t u32[2]; /**< 2 double words */
}Bytes_t;


uint16_t GetUint16(const uint8_t data[]) {
    Bytes_t b;
    b.u8[1] = data[0];
    b.u8[0] = data[1];
    return b.u16[0];
}

uint32_t GetUint32(const uint8_t data[]) {
    Bytes_t b;
    b.u8[3] = data[0];
    b.u8[2] = data[1];
    b.u8[1] = data[2];
    b.u8[0] = data[3];
    return b.u32[0];
}

void SetUint16(uint8_t data[], const uint16_t value) {
    Bytes_t b;
    b.u16[0] = value;
    data[1] = b.u8[0];
    data[0] = b.u8[1];
}

void SetUint32(uint8_t data[], const uint32_t value) {
    Bytes_t b;
    b.u32[0] = value;
    data[3] = b.u8[0];
    data[2] = b.u8[1];
    data[1] = b.u8[2];
    data[0] = b.u8[3];
}
