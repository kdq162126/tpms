/*
 * display_app1.c
 *
 *  Created on: Mar 6, 2023
 *      Author: khanh
 */

#include "app.h"
#include "display_app.h"

#define LCD_BUFFER_LENGTH                       23

static void LcdStartupHandle(void);

static void press0_writeSegmentHandle(SegElement* this, int32_t val);
static void press1_writeSegmentHandle(SegElement* this, int32_t val);
static void press2_writeSegmentHandle(SegElement* this, int32_t val);
static void press3_writeSegmentHandle(SegElement* this, int32_t val);
static void temp0_writeSegmentHandle(SegElement* this, int32_t val);
static void temp1_writeSegmentHandle(SegElement* this, int32_t val);
static void temp2_writeSegmentHandle(SegElement* this, int32_t val);
static void temp3_writeSegmentHandle(SegElement* this, int32_t val);
static void powerSource_writeSegmentHandle(SegElement* this, int32_t val);

static void GetAllTireDataInFlash(TpmsApp *pApp);

static void lcdDriverSendHandle(uint8_t* buff, uint32_t len) {
    I2cHwSend(buff, len);
}

void DisplayAppInit(TpmsApp* pApp) {
    pApp->lcdDriver.state = LCD_ST_SETUP;
    LcdDriverInit(&pApp->lcdDriver);
    pApp->lcdDriver.write = I2cHwSend;

    pApp->tires[0].press.write = press0_writeSegmentHandle;
    pApp->tires[1].press.write = press1_writeSegmentHandle;
    pApp->tires[2].press.write = press2_writeSegmentHandle;
    pApp->tires[3].press.write = press3_writeSegmentHandle;

    pApp->tires[0].temp.write = temp0_writeSegmentHandle;
    pApp->tires[1].temp.write = temp1_writeSegmentHandle;
    pApp->tires[2].temp.write = temp2_writeSegmentHandle;
    pApp->tires[3].temp.write = temp3_writeSegmentHandle;
}

void DisplayTask(void* pv) {
    (void)pv;

    LcdDriver* this = &tpmsApp.lcdDriver;
    DisplayAppInit(&tpmsApp);

    // Initialize Flash to read previous tires data
    IntFlashConfig();
    GetAllTireDataInFlash(&tpmsApp);

    I2cHwConfig();
    vTaskDelay(500);

    while (1) {
        switch (this->state) {
        case LCD_ST_SETUP:
            LcdDriverConfigSettings(this, 0x03);
            tpmsApp.lcdDriver.state = LCD_ST_STARTUP;
            break;
        case LCD_ST_STARTUP:
            // TODO: change buffer len
            memset(this->dataBuff, 0, LCD_BUFFER_LENGTH);
            LcdStartupHandle();
            LcdDriverWriteRamPages(this, LCD_BUFFER_LENGTH);

            tpmsApp.lcdDriver.state = LCD_ST_ACTIVE;
            break;
        case LCD_ST_IDLE:
            break;
        case LCD_ST_ACTIVE:
            // powerSource_writeSegmentHandle(NULL, 119);
//            tpmsApp.tires[0].press.value = 2000;
//            tpmsApp.tires[3].press.value = 2100;
//            tpmsApp.tires[0].temp.value = 23;
//            tpmsApp.tires[3].temp.value = 22;
//            press0_writeSegmentHandle(&tpmsApp.tires[0].press, tpmsApp.tires[0].press.value / 100);
//            press3_writeSegmentHandle(&tpmsApp.tires[3].press, tpmsApp.tires[3].press.value / 100);
//            temp0_writeSegmentHandle(&tpmsApp.tires[0].temp, tpmsApp.tires[0].temp.value);
//            temp3_writeSegmentHandle(&tpmsApp.tires[3].temp, tpmsApp.tires[3].temp.value);

            // TODO: change buffer len
            // for (uint8_t i = 0; i < 255; i++) {
            //     // powerSource_writeSegmentHandle(NULL, i);

            //     press0_writeSegmentHandle(&tpmsApp.tires[0].press, i);
            //     press1_writeSegmentHandle(&tpmsApp.tires[1].press, i);
            //     press2_writeSegmentHandle(&tpmsApp.tires[2].press, i);
            //     press3_writeSegmentHandle(&tpmsApp.tires[3].press, i);

            //     temp0_writeSegmentHandle(&tpmsApp.tires[0].temp, i);
            //     temp1_writeSegmentHandle(&tpmsApp.tires[1].temp, i);
            //     temp2_writeSegmentHandle(&tpmsApp.tires[2].temp, i);
            //     temp3_writeSegmentHandle(&tpmsApp.tires[3].temp, i);

            //     LcdDriverWriteRamPages(this, LCD_BUFFER_LENGTH);
            //     vTaskDelay(100);
            // }

            LcdDriverWriteRamPages(this, LCD_BUFFER_LENGTH);
            vTaskDelay(100);

            break;
        }
        vTaskDelay(1);
    }
}

static void GetAllTireDataInFlash(TpmsApp *pApp) {
    uint8_t postionMapIds[TIRE_NUMBER][8] = { TIRE_LEFT_FRONT_ID, TIRE_RIGHT_FRONT_ID, TIRE_LEFT_BACK_ID , TIRE_RIGHT_BACK_ID };
    uint8_t compareBuffer[8] = {0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff};

    uint32_t size = TIRE_DATA_FLASH_BUFFER_SIZE*TIRE_NUMBER;
    uint8_t data[size];
    memset(data, 0, size);
    memcpy(data, (uint8_t *)SENSOR_START_FLASH_ADDRESS, size);  
    bool needOverwrite = false;

	for (uint8_t i = 0; i < TIRE_NUMBER; i++) {
        uint8_t offset = TIRE_DATA_FLASH_BUFFER_SIZE*i;

        // Check ID storaged in Flash
        if (memcmp(data+offset, compareBuffer, 8) == 0) {
            // ID == [0xff] -> No Data in Flash
            memcpy(data+18*i, postionMapIds[i], 8);
            needOverwrite = true;
            continue;
        }   

        TireParseByteBuffer(&pApp->tires[i], data+offset);
	}

    if (needOverwrite) {
        // TODO: should check result returned
        IntFlashEraseSector(SENSOR_START_FLASH_ADDRESS);
        IntFlashWriteSector(SENSOR_START_FLASH_ADDRESS, size, data);
    }
}

static void LcdStartupHandle(void) {
    Byte* bytes = tpmsApp.lcdDriver.dataBuff;

    bytes[9].bits.b7 = 1;           // Celcicus
    bytes[14].bits.b5 = 1;          // Car Frame
    bytes[15].bits.b5 = 1;          // BAR
    // bytes[12].bits.b4 = 1;          // Voltage

    // powerSource_writeSegmentHandle(NULL, 120);
}

static void press0_writeSegmentHandle(SegElement* this, int32_t val) {
    (void)this;

    Byte* bytes = tpmsApp.lcdDriver.dataBuff;
    const uint8_t digitNum = 2;
    const uint8_t minDigitShow = 2;
    Digit digits[digitNum];

    if (val == -1) {
        SegmentClearDigits(digits, digitNum);

        bytes[15].bits.b4 = 0;           // Tire_0 icon
        bytes[17].bits.b3 = 0;           // Floating point
    }
    else {
        SegmentUpdateDigits(digits, val, digitNum, minDigitShow);

        bytes[15].bits.b4 = 1;           // Tire_0 icon
        bytes[17].bits.b3 = 1;           // Floating point
    }

    for (uint8_t i = 0; i < digitNum; i++) {
        uint8_t byteShift = i * 2;
        bytes[16 + byteShift].bits.b0 = digits[i].a;
        bytes[16 + byteShift].bits.b1 = digits[i].b;
        bytes[16 + byteShift].bits.b2 = digits[i].c;
        bytes[16 + byteShift].bits.b3 = digits[i].d;
        bytes[17 + byteShift].bits.b0 = digits[i].f;
        bytes[17 + byteShift].bits.b1 = digits[i].g;
        bytes[17 + byteShift].bits.b2 = digits[i].e;
    }
}

static void press1_writeSegmentHandle(SegElement* this, int32_t val) {
    (void)this;

    Byte* bytes = tpmsApp.lcdDriver.dataBuff;
    const uint8_t digitNum = 2;
    const uint8_t minDigitShow = 2;
    Digit digits[digitNum];

    if (val == -1) {
        SegmentClearDigits(digits, digitNum);

        bytes[13].bits.b4 = 0;            // Tire 1 icon
        bytes[9].bits.b3 = 0;           // Floating point
    }
    else {
        SegmentUpdateDigits(digits, val, digitNum, minDigitShow);

        bytes[13].bits.b4 = 1;            // Tire 1 icon
        bytes[9].bits.b3 = 1;           // Floating point
    }

    for (uint8_t i = 0; i < digitNum; i++) {
        uint8_t byteShift = i * 2;
        bytes[8 + byteShift].bits.b0 = digits[i].a;
        bytes[8 + byteShift].bits.b1 = digits[i].b;
        bytes[8 + byteShift].bits.b2 = digits[i].c;
        bytes[8 + byteShift].bits.b3 = digits[i].d;
        bytes[9 + byteShift].bits.b0 = digits[i].f;
        bytes[9 + byteShift].bits.b1 = digits[i].g;
        bytes[9 + byteShift].bits.b2 = digits[i].e;
    }
}

static void press2_writeSegmentHandle(SegElement* this, int32_t val) {
    (void)this;

    Byte* bytes = tpmsApp.lcdDriver.dataBuff;
    const uint8_t digitNum = 2;
    const uint8_t minDigitShow = 2;
    Digit digits[digitNum];

    if (val == -1) {
        SegmentClearDigits(digits, digitNum);

        bytes[15].bits.b6 = 0;           // Tire 2 icon
        bytes[2].bits.b7 = 0;           // Floating point
    }
    else {
        SegmentUpdateDigits(digits, val, digitNum, minDigitShow);

        bytes[15].bits.b6 = 1;           // Tire 2 icon
        bytes[2].bits.b7 = 1;           // Floating point
    }

    for (uint8_t i = 0; i < digitNum; i++) {
        uint8_t byteShift = i * 2;
        bytes[3 - byteShift].bits.b4 = digits[i].a;
        bytes[3 - byteShift].bits.b5 = digits[i].b;
        bytes[3 - byteShift].bits.b6 = digits[i].c;
        bytes[3 - byteShift].bits.b7 = digits[i].d;
        bytes[2 - byteShift].bits.b4 = digits[i].f;
        bytes[2 - byteShift].bits.b5 = digits[i].g;
        bytes[2 - byteShift].bits.b6 = digits[i].e;
    }
}

static void press3_writeSegmentHandle(SegElement* this, int32_t val) {
    (void)this;

    Byte* bytes = tpmsApp.lcdDriver.dataBuff;
    const uint8_t digitNum = 2;
    const uint8_t minDigitShow = 2;
    Digit digits[digitNum];

    if (val == -1) {
        SegmentClearDigits(digits, digitNum);

        bytes[13].bits.b5 = 0;            // Tire 3 icon
        bytes[6].bits.b0 = 0;           // Floating point
    }
    else {
        SegmentUpdateDigits(digits, val, digitNum, minDigitShow);

        bytes[13].bits.b5 = 1;            // Tire 3 icon
        bytes[6].bits.b0 = 1;           // Floating point
    }

    for (uint8_t i = 0; i < digitNum; i++) {
        uint8_t byteShift = i * 2;
        bytes[6 - byteShift].bits.b1 = digits[i].e;
        bytes[6 - byteShift].bits.b2 = digits[i].g;
        bytes[6 - byteShift].bits.b3 = digits[i].f;
        bytes[7 - byteShift].bits.b0 = digits[i].d;
        bytes[7 - byteShift].bits.b1 = digits[i].c;
        bytes[7 - byteShift].bits.b2 = digits[i].b;
        bytes[7 - byteShift].bits.b3 = digits[i].a;
    }
}

static void temp0_writeSegmentHandle(SegElement* this, int32_t val) {
    (void)this;

    Byte* bytes = tpmsApp.lcdDriver.dataBuff;
    const uint8_t digitNum = 2;
    const uint8_t minDigitShow = 2;
    Digit digits[digitNum];


    // TODO: Handle negative and 3 digits number
    // bytes[1].bits.b0 = 1; // Negative
    // bytes[3].bits.b0 = 1; // Hundred digit

    if (val == -1) {
        SegmentClearDigits(digits, digitNum);
    }
    else {
        SegmentUpdateDigits(digits, val, digitNum, minDigitShow);
    }

    for (uint8_t i = 0; i < digitNum; i++) {
        uint8_t byteShift = i * 2;
        bytes[16 + byteShift].bits.b4 = digits[i].a;
        bytes[16 + byteShift].bits.b5 = digits[i].b;
        bytes[16 + byteShift].bits.b6 = digits[i].c;
        bytes[16 + byteShift].bits.b7 = digits[i].d;
        bytes[17 + byteShift].bits.b4 = digits[i].f;
        bytes[17 + byteShift].bits.b5 = digits[i].g;
        bytes[17 + byteShift].bits.b6 = digits[i].e;
    }
}

static void temp1_writeSegmentHandle(SegElement* this, int32_t val) {
    (void)this;

    Byte* bytes = tpmsApp.lcdDriver.dataBuff;
    const uint8_t digitNum = 2;
    const uint8_t minDigitShow = 2;
    Digit digits[digitNum];

    // TODO: Handle negative and 3 digits number
    // bytes[20].bits.b4 = 1; // Negative
    // bytes[22].bits.b4 = 1; // Hundred digit

    if (val == -1) {
        SegmentClearDigits(digits, digitNum);
    }
    else {
        SegmentUpdateDigits(digits, val, digitNum, minDigitShow);
    }

    for (uint8_t i = 0; i < digitNum; i++) {
        uint8_t byteShift = i * 2;
        bytes[8 + byteShift].bits.b4 = digits[i].a;
        bytes[8 + byteShift].bits.b5 = digits[i].b;
        bytes[8 + byteShift].bits.b6 = digits[i].c;
        bytes[8 + byteShift].bits.b7 = digits[i].d;
        bytes[9 + byteShift].bits.b4 = digits[i].f;
        bytes[9 + byteShift].bits.b5 = digits[i].g;
        bytes[9 + byteShift].bits.b6 = digits[i].e;
    }
}

static void temp2_writeSegmentHandle(SegElement* this, int32_t val) {
    (void)this;

    Byte* bytes = tpmsApp.lcdDriver.dataBuff;
    const uint8_t digitNum = 2;
    const uint8_t minDigitShow = 2;
    Digit digits[digitNum];


    // TODO: Handle negative and 3 digits number
    // bytes[6].bits.b0 = 1; // Negative
    // bytes[8].bits.b0 = 1; // Hundred digit

    if (val == -1) {
        SegmentClearDigits(digits, digitNum);
    }
    else {
        SegmentUpdateDigits(digits, val, digitNum, minDigitShow);
    }

    for (uint8_t i = 0; i < digitNum; i++) {
        uint8_t byteShift = i * 2;
        bytes[3 - byteShift].bits.b0 = digits[i].a;
        bytes[3 - byteShift].bits.b1 = digits[i].b;
        bytes[3 - byteShift].bits.b2 = digits[i].c;
        bytes[3 - byteShift].bits.b3 = digits[i].d;
        bytes[2 - byteShift].bits.b1 = digits[i].f;
        bytes[2 - byteShift].bits.b2 = digits[i].g;
        bytes[2 - byteShift].bits.b3 = digits[i].e;
    }
}

static void temp3_writeSegmentHandle(SegElement* this, int32_t val) {
    (void)this;

    Byte* bytes = tpmsApp.lcdDriver.dataBuff;
    const uint8_t digitNum = 2;
    const uint8_t minDigitShow = 2;
    Digit digits[digitNum];


    // TODO: Handle negative and 3 digits number
    // bytes[16].bits.b0 = 1; // Negative
    // bytes[18].bits.b0 = 1; // Hundred digit

    if (val == -1) {
        SegmentClearDigits(digits, digitNum);
    }
    else {
        SegmentUpdateDigits(digits, val, digitNum, minDigitShow);
    }

    for (uint8_t i = 0; i < digitNum; i++) {
        uint8_t byteShift = i * 2;
        bytes[7 - byteShift].bits.b4 = digits[i].d;
        bytes[7 - byteShift].bits.b5 = digits[i].c;
        bytes[7 - byteShift].bits.b6 = digits[i].b;
        bytes[7 - byteShift].bits.b7 = digits[i].a;
        bytes[6 - byteShift].bits.b4 = digits[i].e;
        bytes[6 - byteShift].bits.b5 = digits[i].g;
        bytes[6 - byteShift].bits.b6 = digits[i].f;
    }
}

static void powerSource_writeSegmentHandle(SegElement* this, int32_t val) {
    (void)this;

    Byte* bytes = tpmsApp.lcdDriver.dataBuff;
    const uint8_t digitNum = 2;
    const uint8_t minDigitShow = 2;
    Digit digits[digitNum];

    bytes[13].bits.b3 = 1; // Floating point
    if (val >= 100) {
        bytes[15].bits.b3 = 1;
    }
    else {
        bytes[15].bits.b3 = 0;
    }

    SegmentUpdateDigits(digits, val, digitNum, minDigitShow);
    for (uint8_t i = 0; i < digitNum + 1; i++) {
        uint8_t byteShift = i * 2;
        bytes[12 + byteShift].bits.b0 = digits[i].a;
        bytes[12 + byteShift].bits.b1 = digits[i].b;
        bytes[12 + byteShift].bits.b2 = digits[i].c;
        bytes[12 + byteShift].bits.b3 = digits[i].d;
        bytes[13 + byteShift].bits.b0 = digits[i].f;
        bytes[13 + byteShift].bits.b1 = digits[i].g;
        bytes[13 + byteShift].bits.b2 = digits[i].e;
    }
}
