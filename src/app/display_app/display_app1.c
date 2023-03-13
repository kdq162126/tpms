/*
 * display_app1.c
 *
 *  Created on: Mar 6, 2023
 *      Author: khanh
 */

#include "app.h"
#include "display_app.h"

static void press0_writeSegmentHandle(SegElement* this, int32_t val);
static void press1_writeSegmentHandle(SegElement* this, int32_t val);
static void press2_writeSegmentHandle(SegElement* this, int32_t val);
static void press3_writeSegmentHandle(SegElement* this, int32_t val);
static void temp0_writeSegmentHandle(SegElement* this, int32_t val);
static void temp1_writeSegmentHandle(SegElement* this, int32_t val);
static void temp2_writeSegmentHandle(SegElement* this, int32_t val);
static void temp3_writeSegmentHandle(SegElement* this, int32_t val);
static void powerSource_writeSegmentHandle(SegElement* this, int32_t val);


void DisplayAppInit(void) {
    TpmsApp* pApp;

    pApp->lcdDriver.state = LCD_ST_SETUP;
    LcdDriverInit(&pApp->lcdDriver, I2cHwSend);

    pApp->tires[0].press.update = press0_writeSegmentHandle;
    pApp->tires[1].press.update = press1_writeSegmentHandle;
    pApp->tires[2].press.update = press2_writeSegmentHandle;
    pApp->tires[3].press.update = press3_writeSegmentHandle;

    pApp->tires[0].temp.update = temp0_writeSegmentHandle;
    pApp->tires[1].temp.update = temp1_writeSegmentHandle;
    pApp->tires[2].temp.update = temp2_writeSegmentHandle;
    pApp->tires[3].temp.update = temp3_writeSegmentHandle;


}

void DisplayTask(void* pv) {
    (void)pv;

    LcdDriver* this = &tpmsApp.lcdDriver;
    DisplayAppInit();

    I2cHwConfig();
    vTaskDelay(500);

    while (1) {
        switch (this->state) {
        case LCD_ST_SETUP:
            LcdDriverConfigSettings(this, 0x00);
            tpmsApp.lcdDriver.state = LCD_ST_ACTIVE;
            break;
        case LCD_ST_STARTUP:
            // TODO: change buffer len
            memset(this->dataBuff, 0, 16);
            LcdDriverWriteRamPages(this, 16);
            break;
        case LCD_ST_IDLE:
            break;
        case LCD_ST_ACTIVE:
            // TODO: change buffer len
            LcdDriverWriteRamPages(this, 16);
            vTaskDelay(10);
            break;
        }
        vTaskDelay(1);
    }
}

static void press0_writeSegmentHandle(SegElement* this, int32_t val) {
    (void)this;

    Byte* bytes = tpmsApp.lcdDriver.dataBuff;
    const uint8_t digitNum = 2;
    const uint8_t minDigitShow = 2;
    Digit digits[digitNum];

    bytes[9].bits.b7 = 1;           // Tire_0 icon  
    bytes[3].bits.b4 = 1;           // Floating point
    if (val >= 100) {
        bytes[0].bits.b4 = 1;
    }
    else {
        bytes[0].bits.b4 = 0;
    }

    SegmentUpdateDigits(digits, val, digitNum, minDigitShow);
    for (uint8_t i = 0; i <= digitNum; i += 2) {
        bytes[2 - i].bits.b5 = digits[i].e;
        bytes[2 - i].bits.b6 = digits[i].g;
        bytes[2 - i].bits.b7 = digits[i].f;
        bytes[3 - i].bits.b4 = digits[i].d;
        bytes[3 - i].bits.b5 = digits[i].c;
        bytes[3 - i].bits.b6 = digits[i].b;
        bytes[3 - i].bits.b7 = digits[i].a;
    }
}

static void press1_writeSegmentHandle(SegElement* this, int32_t val) {
    (void)this;

    Byte* bytes = tpmsApp.lcdDriver.dataBuff;
    const uint8_t digitNum = 2;
    const uint8_t minDigitShow = 2;
    Digit digits[digitNum];

    bytes[9].bits.b2 = 1;            // Tire 1 icon
    bytes[21].bits.b3 = 1;           // Floating point
    if (val >= 100) {
        bytes[19].bits.b3 = 1;
    }
    else {
        bytes[19].bits.b3 = 0;
    }

    SegmentUpdateDigits(digits, val, digitNum, minDigitShow);
    for (uint8_t i = 0; i <= digitNum; i += 2) {
        bytes[21 - i].bits.b0 = digits[i].f;
        bytes[21 - i].bits.b1 = digits[i].g;
        bytes[21 - i].bits.b2 = digits[i].e;
        bytes[22 - i].bits.b0 = digits[i].a;
        bytes[22 - i].bits.b1 = digits[i].b;
        bytes[22 - i].bits.b2 = digits[i].c;
        bytes[22 - i].bits.b3 = digits[i].d;
    }
}

static void press2_writeSegmentHandle(SegElement* this, int32_t val) {
    (void)this;

    Byte* bytes = tpmsApp.lcdDriver.dataBuff;
    const uint8_t digitNum = 2;
    const uint8_t minDigitShow = 2;
    Digit digits[digitNum];

    bytes[9].bits.b6 = 1;           // Tire 2 icon
    bytes[7].bits.b4 = 1;           // Floating point
    if (val >= 100) {
        bytes[5].bits.b4 = 1;
    }
    else {
        bytes[5].bits.b4 = 0;
    }

    SegmentUpdateDigits(digits, val, digitNum, minDigitShow);
    for (uint8_t i = 0; i <= digitNum; i += 2) {
        bytes[8 - i].bits.b4 = digits[i].d;
        bytes[8 - i].bits.b5 = digits[i].c;
        bytes[8 - i].bits.b6 = digits[i].b;
        bytes[8 - i].bits.b7 = digits[i].a;
        bytes[7 - i].bits.b5 = digits[i].e;
        bytes[7 - i].bits.b6 = digits[i].g;
        bytes[7 - i].bits.b7 = digits[i].f;
    }
}

static void press3_writeSegmentHandle(SegElement* this, int32_t val) {
    (void)this;

    Byte* bytes = tpmsApp.lcdDriver.dataBuff;
    const uint8_t digitNum = 2;
    const uint8_t minDigitShow = 2;
    Digit digits[digitNum];

    bytes[9].bits.b3 = 1;            // Tire 3 icon
    bytes[17].bits.b4 = 1;           // Floating point
    if (val >= 100) {
        bytes[15].bits.b4 = 1;
    }
    else {
        bytes[15].bits.b4 = 0;
    }

    SegmentUpdateDigits(digits, val, digitNum, minDigitShow);
    for (uint8_t i = 0; i < digitNum; i++) {
        bytes[17 - i].bits.b5 = digits[i].e;
        bytes[17 - i].bits.b6 = digits[i].g;
        bytes[17 - i].bits.b7 = digits[i].f;
        bytes[18 - i].bits.b4 = digits[i].d;
        bytes[18 - i].bits.b5 = digits[i].c;
        bytes[18 - i].bits.b6 = digits[i].b;
        bytes[18 - i].bits.b7 = digits[i].a;
    }
}

static void temp0_writeSegmentHandle(SegElement* this, int32_t val) {
    (void)this;

    Byte* bytes = tpmsApp.lcdDriver.dataBuff;
    const uint8_t digitNum = 2;
    const uint8_t minDigitShow = 2;
    Digit digits[digitNum];


    // TODO: Handle negative and 3 digits number
    bytes[1].bits.b0 = 1; // Negative
    bytes[3].bits.b0 = 1; // Hundred digit

    SegmentUpdateDigits(digits, val, digitNum, minDigitShow);
    for (uint8_t i = 0; i <= digitNum; i += 2) {
        bytes[0 + i].bits.b0 = digits[i].a;
        bytes[0 + i].bits.b1 = digits[i].b;
        bytes[0 + i].bits.b2 = digits[i].c;
        bytes[0 + i].bits.b3 = digits[i].d;
        bytes[1 + i].bits.b1 = digits[i].f;
        bytes[1 + i].bits.b2 = digits[i].g;
        bytes[1 + i].bits.b3 = digits[i].e;
    }
}

static void temp1_writeSegmentHandle(SegElement* this, int32_t val) {
    (void)this;

    Byte* bytes = tpmsApp.lcdDriver.dataBuff;
    const uint8_t digitNum = 2;
    const uint8_t minDigitShow = 2;
    Digit digits[digitNum];


    // TODO: Handle negative and 3 digits number
    bytes[20].bits.b4 = 1; // Negative
    bytes[22].bits.b4 = 1; // Hundred digit

    SegmentUpdateDigits(digits, val, digitNum, minDigitShow);
    for (uint8_t i = 0; i <= digitNum; i += 2) {
        bytes[21 - i].bits.b4 = digits[i].a;
        bytes[21 - i].bits.b5 = digits[i].b;
        bytes[21 - i].bits.b6 = digits[i].c;
        bytes[21 - i].bits.b7 = digits[i].d;
        bytes[22 - i].bits.b5 = digits[i].f;
        bytes[22 - i].bits.b6 = digits[i].g;
        bytes[22 - i].bits.b7 = digits[i].e;
    }
}

static void temp2_writeSegmentHandle(SegElement* this, int32_t val) {
    (void)this;

    Byte* bytes = tpmsApp.lcdDriver.dataBuff;
    const uint8_t digitNum = 2;
    const uint8_t minDigitShow = 2;
    Digit digits[digitNum];


    // TODO: Handle negative and 3 digits number
    bytes[6].bits.b0 = 1; // Negative
    bytes[8].bits.b0 = 1; // Hundred digit

    SegmentUpdateDigits(digits, val, digitNum, minDigitShow);
    for (uint8_t i = 0; i <= digitNum; i += 2) {
        bytes[5 + i].bits.b0 = digits[i].a;
        bytes[5 + i].bits.b1 = digits[i].b;
        bytes[5 + i].bits.b2 = digits[i].c;
        bytes[5 + i].bits.b3 = digits[i].d;
        bytes[6 + i].bits.b1 = digits[i].f;
        bytes[6 + i].bits.b2 = digits[i].g;
        bytes[6 + i].bits.b3 = digits[i].e;
    }
}

static void temp3_writeSegmentHandle(SegElement* this, int32_t val) {
    (void)this;

    Byte* bytes = tpmsApp.lcdDriver.dataBuff;
    const uint8_t digitNum = 2;
    const uint8_t minDigitShow = 2;
    Digit digits[digitNum];


    // TODO: Handle negative and 3 digits number
    bytes[16].bits.b0 = 1; // Negative
    bytes[18].bits.b0 = 1; // Hundred digit

    SegmentUpdateDigits(digits, val, digitNum, minDigitShow);
    for (uint8_t i = 0; i <= digitNum; i += 2) {
        bytes[15 + i].bits.b0 = digits[i].a;
        bytes[15 + i].bits.b1 = digits[i].b;
        bytes[15 + i].bits.b2 = digits[i].c;
        bytes[15 + i].bits.b3 = digits[i].d;
        bytes[16 + i].bits.b1 = digits[i].f;
        bytes[16 + i].bits.b2 = digits[i].g;
        bytes[16 + i].bits.b3 = digits[i].e;
    }
}

static void powerSource_writeSegmentHandle(SegElement* this, int32_t val) {
    (void)this;

    Byte* bytes = tpmsApp.lcdDriver.dataBuff;
    const uint8_t digitNum = 2;
    const uint8_t minDigitShow = 2;
    Digit digits[digitNum];


    // TODO: Handle negative and 3 digits number
    bytes[12].bits.b2 = 1; // Floating point
    bytes[10].bits.b3 = 1; // Hundred digit

    SegmentUpdateDigits(digits, val, digitNum, minDigitShow);
    for (uint8_t i = 0; i <= digitNum + 1; i += 3) {
        bytes[14 - i].bits.b0 = digits[i].a;
        bytes[14 - i].bits.b1 = digits[i].b;
        bytes[14 - i].bits.b2 = digits[i].c;
        bytes[14 - i].bits.b3 = digits[i].d;
        bytes[13 - i].bits.b0 = digits[i].f;
        bytes[13 - i].bits.b1 = digits[i].g;
        bytes[13 - i].bits.b2 = digits[i].e;
    }
}
