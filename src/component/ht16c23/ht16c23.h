/*
 * ht16c23.h
 *
 *  Created on: Mar 2, 2023
 *      Author: khanh
 */

#ifndef COMPONENT_HT16C23_HT16C23_H_
#define COMPONENT_HT16C23_HT16C23_H_

#include "segment.h"

#define LCD_BUFFER_LENGTH                       40

#define HT16C23_DISPLAY_DATA_CMD                (0x80)
#define HT16C23_DRIVE_MODE_CMD                  (0x82)
#define HT16C23_SYSTEM_MODE_CMD                 (0x84)
#define HT16C23_FRAME_FREQ_CMD                  (0x86)
#define HT16C23_BLINKING_FREQ_CMD               (0x88)
#define HT16C23_IVA_SETTING_CMD                 (0x8a)

typedef enum LCD_STATE {
    LCD_ST_SETUP = 0,
    LCD_ST_STARTUP,
    LCD_ST_IDLE,
    LCD_ST_ACTIVE
} LCD_STATE;

typedef struct LcdDriver_t LcdDriver;
struct LcdDriver_t {
    LCD_STATE state;
    Byte* dataBuff;
    void (*write)(uint8_t* buff, uint32_t len);
};

void LcdDriverInit(LcdDriver* this, void(*writeHandle)(uint8_t* buff, uint32_t len));
void LcdDriverConfigSettings(LcdDriver* this, uint8_t driveMode);
void LcdDriverWriteRamPages(LcdDriver* this, uint32_t len);

#endif /* COMPONENT_HT16C23_HT16C23_H_ */
