/*
 * string_util.h
 *
 *  Created on: Feb 13, 2023
 *      Author: khanh
 */

#ifndef UTILS_STRING_UTIL_STRING_UTIL_H_
#define UTILS_STRING_UTIL_STRING_UTIL_H_

#include "stdint.h"

char* StringAppendString(char* buff, char* value);
char* StringAppendStringWithQuote(char* buff, char* value);
char* StringAppendInt(char* buff, uint16_t value);
char* StringAppendFloat(char* buff, float value);
char* JsonOpen(char* buff);
char* JsonClose(char* buff);
char* JsonFromInt(char* buff, char* key, uint16_t value);
char* JsonFromFloat(char* buff, char* key, float value);
char* JsonFromString(char* buff, char* key, char* value);
char* JsonFromIntArray(char* buff, char* key, uint16_t value[], uint16_t len);
char* StringJoin(char* buff, char** arr, uint8_t arrLen, char sep);

#endif /* UTILS_STRING_UTIL_STRING_UTIL_H_ */
