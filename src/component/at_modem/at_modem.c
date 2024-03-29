/*
 * at_modem.c
 *
 *  Created on: Feb 14, 2023
 *      Author: khanh
 */

#include "at_modem.h"
#include "string.h"

static void ATmodemClean(ATmodem* this);

char rxBuffer[AT_MODEM_RX_BUFFER_LEN];

void ATmodemInit(ATmodem* this, void (*sendHandle)(const char*), uint32_t* timestampSource, void (*delayMs)(const uint32_t)) {
    this->receiveData = rxBuffer;
    this->sendCmd = sendHandle;
    this->clock.timestampMs = timestampSource;
    this->delayMs = delayMs;
}

// Retry if command was excutived fail
bool ATmodemExcutiveCmd(ATmodem* this, const char* cmd, char* resp, uint32_t timeoutMs, uint8_t retryNum) {
    this->expectMsg = resp;

    for (uint8_t retryCnt = 0; retryCnt < retryNum; retryCnt++) {
        ATmodemClean(this);
        ClockEnableTimeout(&this->clock, timeoutMs);

        this->sendCmd(cmd);
        while (!(this->isResponse || ClockIsTimeout(&this->clock)));
        this->delayMs(100);

        if (ClockIsTimeout(&this->clock))
            continue;

        ClockDisableTimeout(&this->clock);
        return true;
    }
    return false;
}


static void ATmodemClean(ATmodem* this) {
    this->isResponse = false;
    memset(this->receiveData, '\0', AT_MODEM_RX_BUFFER_LEN);
}

