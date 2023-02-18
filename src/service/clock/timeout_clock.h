/*
 * timeout_clock.h
 *
 *  Created on: Feb 17, 2023
 *      Author: khanh
 */

#ifndef SERVICE_CLOCK_TIMEOUT_CLOCK_H_
#define SERVICE_CLOCK_TIMEOUT_CLOCK_H_

#include "stdbool.h"
#include "stdint.h"

typedef struct Clock_t Clock;
struct Clock_t {
    bool isActive;
    bool isTimeout;
    uint32_t timeoutMs;
    uint32_t* timestampMs; // Notice: this pointer must be assigned to SystemTimestamp
};

void ClockInit(Clock* this, uint32_t* timestampSource);
void ClockEnableTimeout(Clock* this, const uint32_t timeoutMs);
void ClockDisableTimeout(Clock* this);

static inline void ClockProcessTimeout(Clock* this) {
    if (!this->isActive)
        return;
    if (*this->timestampMs <= this->timeoutMs)
        return;

    this->isTimeout = true;
    this->isActive = false;
}

static inline bool ClockIsTimeout(Clock* this) {
    return this->isTimeout;
}

#endif /* SERVICE_CLOCK_TIMEOUT_CLOCK_H_ */
