/*
 * at_modem.h
 *
 *  Created on: Feb 14, 2023
 *      Author: khanh
 */

#ifndef COMPONENT_AT_MODEM_AT_MODEM_H_
#define COMPONENT_AT_MODEM_AT_MODEM_H_

#include "stdint.h"
#include "string.h"
#include "timeout_clock.h"

#define AT_MODEM_BUILD_HEAD(buff) \
    *buff++ = 'A';                \
    *buff++ = 'T';                \
    *buff++ = '+'
#define AT_MODEM_BUILD_TAIL(buff) \
    *buff++ = '\r';               \
    *buff++ = '\n';               \
    *buff++ = '\0'

#define AT_MODEM_RX_BUFFER_LEN 100UL

typedef struct ATmodem_t ATmodem;
struct ATmodem_t {
    volatile bool isResponse;
    char* expectMsg;
    char* receiveData;
    Clock clock;
    void (*sendCmd)(const char* cmd);
};

void ATmodemInit(ATmodem* this, void (*sendHandle)(const char*), uint32_t* timestampSource);
bool ATmodemExcutiveCmd(ATmodem* this, const char* cmd, const char* resp, uint32_t timeoutMs, uint8_t retryNum);

static inline void ATmodemDetectNewMessage(ATmodem* this, char* rxBuff) {
    memcpy(this->receiveData, rxBuff, strlen(rxBuff));
    this->isResponse = true;
}

#endif /* COMPONENT_AT_MODEM_AT_MODEM_H_ */
