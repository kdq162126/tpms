#include "timeout_clock.h"

void ClockInit(Clock* this, uint32_t* timestampSource) {
    this->isActive = false;
    this->isTimeout = false;
    this->timestampMs = timestampSource;
}

void ClockEnableTimeout(Clock* this, const uint32_t timeoutMs) {
    this->timeoutMs = *this->timestampMs + timeoutMs;
    this->isActive = true;
    this->isTimeout = false;
}

void ClockDisableTimeout(Clock* this) {
    this->isActive = false;
    this->isTimeout = false;
}
