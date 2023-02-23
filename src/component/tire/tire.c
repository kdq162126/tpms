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
    buff = JsonFromString(buff, "tireId", this->Id);
    *buff++ = ',';
    buff = JsonFromString(buff, "tireName", this->Name);
    *buff++ = ',';
    buff = JsonFromString(buff, "position", this->position);
    *buff++ = ',';
    buff = JsonFromInt(buff, "pressure", this->pressure);
    *buff++ = ',';
    buff = JsonFromInt(buff, "batteryLevel", this->batPercent);
    *buff++ = ',';
    buff = JsonFromInt(buff, "temperature", this->temp);
    buff = JsonClose(buff);

    return buff;
}

void TireSetId(Tire* this, char* id) {
    memset(this->Id, 0, 5);
    memcpy(this->Id, id, strlen(id));
}
