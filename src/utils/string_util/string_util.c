/*
 * string_util.c
 *
 *  Created on: Feb 13, 2023
 *      Author: khanh
 */

#include "string_util.h"
#include "stdio.h"
#include "stdint.h"

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

char* StringAppendInt(char* buff, uint16_t value) {
    char numStr[16];
    sprintf(numStr, "%d", value);
    buff = StringAppendString(buff, numStr);
    return buff;
}

char* StringAppendFloat(char* buff, float value) {
    char numStr[16];
    sprintf(numStr, "%.8f", value);
    buff = StringAppendString(buff, numStr);
    return buff;
}

char* JsonOpen(char* buff) {
    *buff++ = '{';
    return buff;
}

char* JsonClose(char* buff) {
    *buff++ = '}';
    return buff;
}

char* JsonFromInt(char* buff, char* key, uint16_t value) {
    buff = StringAppendStringWithQuote(buff, key);
    *buff++ = ':';
    buff = StringAppendInt(buff, value);
    return buff;
}

char* JsonFromFloat(char* buff, char* key, float value) {
    buff = StringAppendStringWithQuote(buff, key);
    *buff++ = ':';
    buff = StringAppendFloat(buff, value);
    return buff;
}

char* JsonFromString(char* buff, char* key, char* value) {
    buff = StringAppendStringWithQuote(buff, key);
    *buff++ = ':';
    buff = StringAppendStringWithQuote(buff, value);
    return buff;
}

char* JsonFromIntArray(char* buff, char* key, uint16_t value[], uint16_t len) {
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

