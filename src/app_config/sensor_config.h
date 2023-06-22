#include "sdk_project_config.h"

#define TIRE_LEFT_FRONT_ID              "C9524BE6"
#define TIRE_RIGHT_FRONT_ID             "C9524277"
#define TIRE_LEFT_BACK_ID               "C9525091"
#define TIRE_RIGHT_BACK_ID              "C9524CCF"

#define SENSOR_START_FLASH_ADDRESS		FEATURE_FLS_PF_BLOCK_SIZE - FEATURE_FLS_PF_BLOCK_SECTOR_SIZE
// Memory layout: 18 bytes/tire
//	8 bytes: Tire ID
//	3 bytes: Position
//  4 bytes: Pressure
//	2 bytes: Temperature
//	1 bytes: SoC
