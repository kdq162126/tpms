/*
 * tire.h
 *
 *  Created on: Feb 22, 2023
 *      Author: khanh
 */

#ifndef COMPONENT_TIRE_TIRE_H_
#define COMPONENT_TIRE_TIRE_H_

#include "stdint.h"
#include "string_util.h"

typedef struct Tire_t Tire;
struct Tire_t {
    char Id[5];
    char pos[5];
    uint8_t press;
    uint8_t temp;
    uint8_t bat;
};

void TireInit(Tire* this);
char* TirePackageJsonMessage(Tire* this, char* buff);
void TireSetId(Tire* this, char* id);

#endif /* COMPONENT_TIRE_TIRE_H_ */
