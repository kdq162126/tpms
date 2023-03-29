/*
 * string_util.c
 *
 *  Created on: Feb 13, 2023
 *      Author: khanh
 */

#include "string_util.h"
#include "stdint.h"
#include "stdlib.h"
#include "math.h"
#include "string.h"

static const uint8_t digit_char[10] = { '0','1','2','3','4','5','6','7','8','9' };

void StringSwap(const char* p_buffer, char* p_msg, const uint8_t len) {
    uint8_t i = 0;
    for (i = 0;i < len;i++) {
        p_msg[i] = p_buffer[len - i - 1];
    }
}

int LongToString(const uint32_t data, char* s) {
    uint32_t remain = data;
    uint8_t digit = 0;
    uint8_t index = 0;
    char buffer[16] = { 0 };
    while (remain >= 10) {
        digit = (uint8_t)(remain % 10);
        buffer[index] = (char)digit_char[digit];
        index++;
        remain = (uint32_t)(remain / 10);
    }
    buffer[index] = (char)digit_char[remain];
    StringSwap(buffer, s, index + 1);
    s[index + 1] = '\0';
    return (index + 1);
}

char* StringAppendString(char* buff, char* value) {
    while (*value) {
        *buff++ = *value++;
    }
    return buff;
}

char* StringAppendStringWithQuote(char* buff, char* value) {
    *buff++ = '"';
    while (*value) {
        *buff++ = *value++;
    }
    *buff++ = '"';
    return buff;
}

char* StringAppendInt(char* buff, uint32_t value) {
    char numStr[16] = { 0 };
    LongToString(value, numStr);
    buff = StringAppendString(buff, numStr);
    return buff;
}

char* StringAppendFloat(char* buff, const float val, uint8_t afterPoint) {
    uint32_t iPart = (uint32_t)val;
    buff = StringAppendInt(buff, iPart);
    *buff++ = '.';

    float   fPart = val - (float)iPart;
    uint32_t fPartInt = (uint32_t)(fPart * pow(10, afterPoint + 1));
    uint8_t index = (uint8_t)LongToString(fPartInt, buff) - 1;
    if (index < afterPoint) {
        memset(buff, '0', afterPoint - index);
        buff += afterPoint - index;
    }
    buff = StringAppendInt(buff, fPartInt);
    return (buff);
}

// char* StringAppendFloat(char* buff, float value) {
//     char numStr[16];
//     gcvt(value, 12, numStr);
//     buff = StringAppendString(buff, numStr);
//     return buff;
// }

char* JsonOpen(char* buff) {
    *buff++ = '{';
    return buff;
}

char* JsonClose(char* buff) {
    *buff++ = '}';
    return buff;
}

char* JsonFromInt(char* buff, char* key, uint32_t value) {
    buff = StringAppendStringWithQuote(buff, key);
    *buff++ = ':';
    buff = StringAppendInt(buff, value);
    return buff;
}

char* JsonFromFloat(char* buff, char* key, float value, uint8_t afterPoint) {
    buff = StringAppendStringWithQuote(buff, key);
    *buff++ = ':';
    buff = StringAppendFloat(buff, value, afterPoint);
    return buff;
}

char* JsonFromString(char* buff, char* key, char* value) {
    buff = StringAppendStringWithQuote(buff, key);
    *buff++ = ':';
    buff = StringAppendStringWithQuote(buff, value);
    return buff;
}

char* JsonFromIntArray(char* buff, char* key, uint32_t value[], uint16_t len) {
    buff = StringAppendStringWithQuote(buff, key);
    *buff++ = ':';
    *buff++ = '[';
    for (uint16_t i = 0; i < len; i++) {
        buff = StringAppendInt(buff, value[i]);
        *buff++ = ',';
    }
    *(--buff) = ']';
    buff++;
    return buff;
}

char* StringJoin(char* buff, char** arr, uint8_t arrLen, char sep) {
    for (uint8_t i = 0; i < arrLen; i++) {
        char* str = arr[i];
        buff = StringAppendString(buff, str);
        *buff++ = sep;
    }
    *(--buff) = '\0';
    return buff;
}

void btox(char* xp, const char* bb, int n)
{
    const char xx[] = "0123456789ABCDEF";
    while (--n >= 0) xp[n] = xx[(bb[n >> 1] >> ((1 - (n & 1)) << 2)) & 0xF];
}