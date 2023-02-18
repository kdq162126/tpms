/*
 * core_hw.c
 *
 *  Created on: Feb 15, 2023
 *      Author: khanh
 */

#include "core_hw.h"
#include "sdk_project_config.h"

void SystemClockConfig() {
    CLOCK_SYS_Init(g_clockManConfigsArr, CLOCK_MANAGER_CONFIG_CNT,
        g_clockManCallbacksArr, CLOCK_MANAGER_CALLBACK_CNT);
    CLOCK_SYS_UpdateConfiguration(0U, CLOCK_MANAGER_POLICY_AGREEMENT);
}

void SystemPinsConfig() {
    PINS_DRV_Init(NUM_OF_CONFIGURED_PINS0, g_pin_mux_InitConfigArr0);
}
