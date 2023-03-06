/*
 * board.h
 *
 *  Created on: Feb 17, 2023
 *      Author: khanh
 */

#ifndef BOARD_BOARD_H_
#define BOARD_BOARD_H_

#include "core_hw.h"
#include "uart_hw.h"
#include "i2c_hw.h"

#define LTE_UART_INTERRUPT_PRIORITY             (1)

void BoardConfig(void);

#endif /* BOARD_BOARD_H_ */
