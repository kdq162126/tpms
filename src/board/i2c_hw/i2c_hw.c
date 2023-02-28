/*
 * i2c_hw.c
 *
 *  Created on: Feb 28, 2023
 *      Author: khanh
 */

#include "i2c_hw.h"
#include "sdk_project_config.h"

static volatile bool sendCompleted;
flexio_i2c_master_state_t i2cMasterState;

void I2cHwConfig(void) {
    /* Init the FLEXIO device */
    flexio_device_state_t flexIODeviceState;
    FLEXIO_DRV_InitDevice(INST_FLEXIO_I2C_CONFIG_1, &flexIODeviceState);

    FLEXIO_I2C_DRV_MasterSetSlaveAddr(&i2cMasterState, I2C_SLAVE_ADDRESS);
    FLEXIO_I2C_DRV_MasterInit(INST_FLEXIO_I2C_CONFIG_1, &Flexio_i2c_Config0, &i2cMasterState);
}

void I2cHwSend(uint8_t* buff, uint8_t size) {
    sendCompleted = false;
    FLEXIO_I2C_DRV_MasterSendData(&i2cMasterState, buff, size, true);
    while (!sendCompleted);
}


void flexio_i2c_Transmit_Callback(i2c_master_event_t event, void* userData) {
    (void)userData;

    if (event != I2C_MASTER_EVENT_END_TRANSFER) return;
    sendCompleted = true;
}