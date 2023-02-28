/*
 * i2c_hw.h
 *
 *  Created on: Feb 28, 2023
 *      Author: khanh
 */

#ifndef BOARD_I2C_HW_I2C_HW_H_
#define BOARD_I2C_HW_I2C_HW_H_

#include "stdint.h"

#define I2C_SLAVE_ADDRESS                   (0x7c)

void I2cHwConfig(void);
void I2cHwSend(uint8_t* buff, uint8_t size);

#endif /* BOARD_I2C_HW_I2C_HW_H_ */
