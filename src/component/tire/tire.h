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
#include "segment.h"

typedef enum TIRE_STATE {
    TIRE_ST_INACTIVE = 0,
    TIRE_ST_ACTIVE,
    TIRE_ST_FAIL
} TIRE_STATE;

typedef struct Tire_t Tire;
struct Tire_t {
    TIRE_STATE state;
    char id[8];
    char pos[3];
    SegElement press;
    SegElement temp;
    uint32_t bat;
};

void TireInit(Tire* this);
char* TirePackageJsonMessage(Tire* this, char* buff);
void TireSetId(Tire* this, char* id);
uint32_t TireGetPressure(uint32_t press);

#endif /* COMPONENT_TIRE_TIRE_H_ */
