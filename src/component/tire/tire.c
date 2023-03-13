/*
 * tire.c
 *
 *  Created on: Feb 22, 2023
 *      Author: khanh
 */

#include "tire.h"
#include "string.h"

void TireInit(Tire* this) {
	(void)this;
}

char* TirePackageJsonMessage(Tire* this, char* buff) {
    buff = JsonOpen(buff);
    buff = JsonFromString(buff, "id", this->id);
    *buff++ = ',';
    buff = JsonFromString(buff, "pos", this->pos);
    *buff++ = ',';
    buff = JsonFromInt(buff, "press", this->press.value);
    *buff++ = ',';
    buff = JsonFromInt(buff, "bat", this->bat);
    *buff++ = ',';
    buff = JsonFromInt(buff, "temp", this->temp.value);
    buff = JsonClose(buff);

    return buff;
}

void TireSetId(Tire* this, char* id) {
    memset(this->id, 0, 5);
    memcpy(this->id, id, strlen(id));
}
