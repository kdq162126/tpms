/*
 * ht16c23.c
 *
 *  Created on: Mar 2, 2023
 *      Author: khanh
 */

#include "ht16c23.h"

static void lcdDriverWriteCmd(LcdDriver* this, uint8_t cmd, uint8_t arg);

static Byte _txBuffer[72] = {
    {.val = HT16C23_DISPLAY_DATA_CMD},
    {.val = 0x00},
};

void LcdDriverInit(LcdDriver* this, void(*writeHandle)(uint8_t* buff, uint32_t len)) {
    this->dataBuff = &_txBuffer[2];
    this->write = writeHandle;
}

void LcdDriverConfigSettings(LcdDriver* this, uint8_t driverMode) {
    lcdDriverWriteCmd(this, HT16C23_DRIVE_MODE_CMD, driverMode);
    lcdDriverWriteCmd(this, HT16C23_FRAME_FREQ_CMD, 0x00);           // 80Hz
    lcdDriverWriteCmd(this, HT16C23_IVA_SETTING_CMD, 0x30);          // VLCD ON
    lcdDriverWriteCmd(this, HT16C23_BLINKING_FREQ_CMD, 0x00);        // Blink OFF
    lcdDriverWriteCmd(this, HT16C23_SYSTEM_MODE_CMD, 0x03);          // Internal Oscillator and LCD Display ON
}

void LcdDriverWriteRamPages(LcdDriver* this, uint32_t len) {
    this->write((uint8_t*)_txBuffer, len);
}

static void lcdDriverWriteCmd(LcdDriver* this, uint8_t cmd, uint8_t arg) {
    uint8_t dataBuff[2] = { cmd, arg };
    this->write(dataBuff, 2);
}
