/*
 * tire.c
 *
 *  Created on: Feb 22, 2023
 *      Author: khanh
 */

#include "tire.h"
#include "string.h"

void TireInit(Tire* this) {

}

char* TirePackageJsonMessage(Tire* this, char* buff) {
    buff = JsonOpen(buff);
    buff = JsonFromString(buff, "id", this->Id);
    *buff++ = ',';
    buff = JsonFromString(buff, "pos", this->pos);
    *buff++ = ',';
    buff = JsonFromInt(buff, "press", this->press);
    *buff++ = ',';
    buff = JsonFromInt(buff, "bat", this->bat);
    *buff++ = ',';
    buff = JsonFromInt(buff, "temp", this->temp);
    buff = JsonClose(buff);

    return buff;
}

void TireSetId(Tire* this, char* id) {
    memset(this->Id, 0, 5);
    memcpy(this->Id, id, strlen(id));
}
