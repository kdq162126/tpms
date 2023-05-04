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
    uint32_t inactive_counter;
    char id[10];
    char pos[4];
    SegElement press;
    SegElement temp;
    uint32_t soc;
};

void TireInit(Tire* this);
char* TirePackageJsonMessage(Tire* this, char* buff);
void TireSetId(Tire* this, char* id);
uint32_t TireGetPressure(uint32_t press);
uint32_t TireGetSoc(uint32_t volx100);

static inline void TireSetState(Tire* this, TIRE_STATE state) {
    this->state = state;
    this->inactive_counter = 0;
}

#endif /* COMPONENT_TIRE_TIRE_H_ */
