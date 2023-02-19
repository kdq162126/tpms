#include "board.h"
#include "sdk_project_config.h"

static void InterruptPriorityConfig(void);

void BoardConfig() {
    SystemClockConfig();
    InterruptPriorityConfig();
    SystemPinsConfig();
}

static void InterruptPriorityConfig() {
    INT_SYS_SetPriority(FLEXIO_IRQn, LTE_UART_INTERRUPT_PRIORITY);
}