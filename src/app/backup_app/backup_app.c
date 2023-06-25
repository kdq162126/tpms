/*
 * backup_app.c
 *
 *  Created on: Jun 25, 2023
 *      Author: khanh
 */

#include "app.h"
#include "backup_app.h"
#include "stdlib.h"

bool IsNeedBackupSensorData(TpmsApp* this);

uint32_t _size = TIRE_DATA_FLASH_BUFFER_SIZE*TIRE_NUMBER;
uint8_t _data[TIRE_DATA_FLASH_BUFFER_SIZE*TIRE_NUMBER];

void BackupDataTask(void* pv) {
	(void)pv;

    memset(_data, 0, _size);

	while(1) {
		vTaskDelay(600000);

		if (!IsNeedBackupSensorData(&tpmsApp)) continue;

		for (uint8_t i = 0; i < TIRE_NUMBER; i++) {
	    	uint8_t offset = TIRE_DATA_FLASH_BUFFER_SIZE*i;
	    	TirePackageByteBuffer(&tpmsApp.tires[i], _data+offset);
		}
        IntFlashEraseSector(SENSOR_START_FLASH_ADDRESS);
        IntFlashWriteSector(SENSOR_START_FLASH_ADDRESS, _size, _data);
	}
}

bool IsNeedBackupSensorData(TpmsApp* this) {
    memcpy(_data, (uint8_t *)SENSOR_START_FLASH_ADDRESS, _size);

    Tire tempTires[TIRE_NUMBER];
    for (uint8_t i = 0; i < TIRE_NUMBER; i++) {
    	Tire* tire = &this->tires[i];
    	Tire* tempTire = &tempTires[i];

    	uint8_t offset = TIRE_DATA_FLASH_BUFFER_SIZE*i;
    	TireParseByteBuffer(tempTire, _data+offset);

    	if (IsNeedBackupTireData(tire, tempTire)) {
    		return true;
    	}
    }

    return false;
}



