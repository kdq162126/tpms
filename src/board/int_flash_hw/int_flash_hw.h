/*
 * int_flash_hw.h
 *
 *  Created on: Jun 18, 2023
 *      Author: khanh
 */

#ifndef BOARD_INT_FLASH_HW_INT_FLASH_HW_H_
#define BOARD_INT_FLASH_HW_INT_FLASH_HW_H_

#include "stdbool.h"
#include "stdint.h"

void IntFlashConfig(void);
bool IntFlashWriteSector(uint32_t address, uint32_t size, uint8_t* data);
bool IntFlashEraseSector(uint32_t address);
uint8_t* IntFlashReadData(uint32_t address, uint32_t size, uint8_t* buff);

#endif /* BOARD_INT_FLASH_HW_INT_FLASH_HW_H_ */
