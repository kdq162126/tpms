/*
 * uart_hw.c
 *
 *  Created on: Feb 15, 2023
 *      Author: khanh
 */

#include "uart_hw.h"
#include "string.h"
#include "sdk_project_config.h"

flexio_uart_state_t   uartStateTX, uartStateRX;
flexio_device_state_t flexIODeviceState;
UartHw ec200uHw;

void UartHwConfig(UartHw* this) {
    /* Init the FLEXIO device */
    FLEXIO_DRV_InitDevice(INST_FLEXIO_UART_CONFIG_1, &flexIODeviceState);

    FLEXIO_UART_DRV_Init(INST_FLEXIO_UART_CONFIG_1, &Flexio_uart_Config0_TX, &uartStateTX);
    FLEXIO_UART_DRV_Init(INST_FLEXIO_UART_CONFIG_1, &Flexio_uart_Config0_RX, &uartStateRX);

    FLEXIO_UART_DRV_ReceiveData(&uartStateRX, (uint8_t*)this->rxBuff, 1U);
}

void uartHwClearRxBuffer(UartHw* this) {
    this->rxIndex = 0;
    uartStateRX.rxData = (uint8_t*)this->rxBuff;
}

void UartHwSendMsg(UartHw* this, const char* msg) {
    uartHwClearRxBuffer(this);
    // FLEXIO_UART_DRV_SendDataBlocking(&uartStateTX, (uint8_t*)msg, strlen(msg), 100U);
    FLEXIO_UART_DRV_SendData(&uartStateTX, (uint8_t*)msg, strlen(msg));
    while (FLEXIO_UART_DRV_GetStatus(&uartStateTX, NULL) != STATUS_SUCCESS);
}

void flexio_uart_RX_Callback0(void* driverState, uart_event_t event, void* userData)
{
    (void)userData;
    (void)driverState;

    if (event != UART_EVENT_RX_FULL) return;

    if (ec200uHw.rxIndex >= UART_HW_RX_BUFFER_LEN)
        ec200uHw.rxIndex = 0;
    ec200uHw.rxIndex++;
    FLEXIO_UART_DRV_SetRxBuffer(&uartStateRX, (uint8_t*)&ec200uHw.rxBuff[ec200uHw.rxIndex], 1UL);
    ec200uHw.receive_handle(&ec200uHw);
}

void flexio_uart_TX_Callback0(void* driverState, uart_event_t event, void* userData) {
    (void)userData;
    (void)driverState;
}

// void uartHwSendMsg(UartHw* this, const char* msg);
// void ec00uRxCallback(void* driverState, uart_event_t event, void* userData);

// UartHw ec200uHw = {
//     .sdkInstance = INST_LPUART_1,
//     .sdkState = &lpUartState1,
//     .sdkInitConfig = (void*)&lpuart_1_InitConfig0,
//     .send = uartHwSendMsg,
// };

// void UartHwConfig(UartHw* this) {
//     LPUART_DRV_Init(this->sdkInstance, this->sdkState, this->sdkInitConfig);
//     // FIXME: Temporary cast ec200uRxCallback directly into UartHwConfig,
//     // must be fixed if use more than 1 uart interface or refactor code
//     LPUART_DRV_InstallRxCallback(this->sdkInstance, ec00uRxCallback, this);
// }

// // Clear RxBuff before send message
// void uartHwSendMsg(UartHw* this, const char* msg) {
//     this->rxIndex = 0;
//     memset(this->rxBuff, 0, UART_HW_RX_BUFFER_LEN);
//     LPUART_DRV_SendDataBlocking(this->sdkInstance, (uint8_t*)msg, strlen(msg), UART_HW_TX_TIMEOUT_mS);
// }

// void ec00uRxCallback(void* driverState, uart_event_t event, void* userData)
// {
//     /* Unused parameters */
//     (void)driverState;
//     UartHw* this = (UartHw*)userData;

//     /* Check the event type */
//     if (event == UART_EVENT_RX_FULL)
//     {
//         /* The reception stops when null is received or the buffer is full */
//         if ((this->rxBuff[this->rxIndex] != '\0') && (this->rxIndex != (UART_HW_RX_BUFFER_LEN - 2U))) {
//             /* Update the buffer index and the rx buffer */
//             this->rxIndex++;
//             LPUART_DRV_SetRxBuffer(this->sdkInstance, (uint8_t*)&this->rxBuff[this->rxIndex], 1U);
//             // Handle logic in Application layer
//             if (this->receive_handle != NULL) {
//                 this->receive_handle(this);
//             }
//         }
//     }
// }
