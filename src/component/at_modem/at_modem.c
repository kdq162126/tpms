/*
 * at_modem.c
 *
 *  Created on: Feb 14, 2023
 *      Author: khanh
 */

#include "at_modem.h"
#include "string.h"

static bool ATmodemCheckResponse(ATmodem* this, const char* resp);
static void ATmodemClean(ATmodem* this);

char rxBuffer[AT_MODEM_RX_BUFFER_LEN];

void ATmodemInit(ATmodem* this) {
    this->rxBuff = rxBuffer;
}

bool ATmodemExcutiveCmd(ATmodem* this, const char* cmd, const char* resp, uint32_t timeoutMs) {
    ATmodemClean(this);
    ClockEnableTimeout(&this->clock, timeoutMs);

    this->sendCmd(cmd);
    while (!(this->isResponse || ClockIsTimeout(&this->clock)));

    if (ClockIsTimeout(&this->clock))
        return false;

    ClockDisableTimeout(&this->clock);
    return ATmodemCheckResponse(this, resp);
}

static bool ATmodemCheckResponse(ATmodem* this, const char* resp) {
	char* ret = strstr(this->rxBuff, resp);
    return (ret != NULL);
}

static void ATmodemClean(ATmodem* this) {
    this->isResponse = false;
    memset(this->rxBuff, '\0', AT_MODEM_RX_BUFFER_LEN);
}

