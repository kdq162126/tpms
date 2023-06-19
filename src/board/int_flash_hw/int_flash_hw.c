/*
 * int_flash_hw.c
 *
 *  Created on: Jun 18, 2023
 *      Author: khanh
 */

#include "int_flash_hw.h"
#include "sdk_project_config.h"
#include "string.h"

flash_ssd_config_t flashSSDConfig;

void IntFlashConfig(void) {
    FLASH_DRV_Init(&Flash_InitConfig0, &flashSSDConfig);
    FLASH_DRV_SetFlexRamFunction(&flashSSDConfig, EEE_ENABLE, 0x00u, NULL);
}

bool IntFlashWriteSector(uint32_t address, uint32_t size, uint8_t* data) {
	status_t ret = FLASH_DRV_Program(&flashSSDConfig, address, size, data);
   if (ret != STATUS_SUCCESS) {
	   return false;
   }

    /* Verify the program operation at margin level value of 1, user margin */
    uint32_t failAddr;
    ret = FLASH_DRV_ProgramCheck(&flashSSDConfig, address, size, data, &failAddr, 1u);
    if (ret != STATUS_SUCCESS) {
 	   return false;
    }

    return true;
}

bool IntFlashEraseSector(uint32_t address) {
    address = FEATURE_FLS_PF_BLOCK_SIZE - FEATURE_FLS_PF_BLOCK_SECTOR_SIZE;
    uint32_t size = FEATURE_FLS_PF_BLOCK_SECTOR_SIZE;

    status_t ret = FLASH_DRV_EraseSector(&flashSSDConfig, address, size);
    if (ret != STATUS_SUCCESS) {
    	return false;
    }

    ret = FLASH_DRV_VerifySection(&flashSSDConfig, address, size / FTFx_DPHRASE_SIZE, 1u);
    if (ret != STATUS_SUCCESS) {
    	return false;
    }

    return true;
}

uint8_t* IntFlashReadData(uint32_t address, uint32_t size, uint8_t* buff) {
	memcpy(buff, (uint8_t *)address, size);
	return buff;
}
