/*
 * uart_hw.h
 *
 *  Created on: Feb 15, 2023
 *      Author: khanh
 */

#ifndef BOARD_UART_HW_UART_HW_H_
#define BOARD_UART_HW_UART_HW_H_

#include "stdint.h"

#define UART_HW_TX_TIMEOUT_mS                   500UL
#define UART_HW_RX_BUFFER_LEN                   100UL

typedef struct UartHw_t UartHw;
struct UartHw_t {
    // uint8_t sdkInstance;
    // void* sdkState;
    // void* sdkInitConfig;
    volatile uint8_t rxIndex;
    char rxBuff[UART_HW_RX_BUFFER_LEN];
    void (*send)(UartHw*, const char* msg);
    void (*receive_handle)(UartHw*);    // TODO: Must be assign in app_init
};

extern UartHw ec200uHw;

void UartHwInit(void);

static inline void UartHwSend(UartHw* this, const char* msg) {
    this->send(this, msg);
}

#endif /* BOARD_UART_HW_UART_HW_H_ */
