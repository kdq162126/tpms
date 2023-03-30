/*
 * display_app1.c
 *
 *  Created on: Mar 6, 2023
 *      Author: khanh
 */

#include "app.h"
#include "display_app.h"

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
            tpmsApp.tires[0].press.value = 2000;
            tpmsApp.tires[3].press.value = 2100;
            tpmsApp.tires[0].temp.value = 23;
            tpmsApp.tires[3].temp.value = 22;
            press0_writeSegmentHandle(&tpmsApp.tires[0].press, tpmsApp.tires[0].press.value / 100);
            press3_writeSegmentHandle(&tpmsApp.tires[3].press, tpmsApp.tires[3].press.value / 100);
            temp0_writeSegmentHandle(&tpmsApp.tires[0].temp, tpmsApp.tires[0].temp.value);
            temp3_writeSegmentHandle(&tpmsApp.tires[3].temp, tpmsApp.tires[3].temp.value);

            // TODO: change buffer len
//            for (uint8_t i = 0; i < 255; i++) {
//                press0_writeSegmentHandle(&tpmsApp.tires[0].press, i);
//                press1_writeSegmentHandle(&tpmsApp.tires[1].press, i);
//                press2_writeSegmentHandle(&tpmsApp.tires[2].press, i);
//                press3_writeSegmentHandle(&tpmsApp.tires[3].press, i);
//
//                temp0_writeSegmentHandle(&tpmsApp.tires[0].temp, i);
//                temp1_writeSegmentHandle(&tpmsApp.tires[1].temp, i);
//                temp2_writeSegmentHandle(&tpmsApp.tires[2].temp, i);
//                temp3_writeSegmentHandle(&tpmsApp.tires[3].temp, i);
//
//                powerSource_writeSegmentHandle(NULL, i);
//
//                LcdDriverWriteRamPages(this, LCD_BUFFER_LENGTH);
//                vTaskDelay(100);
//            }

            LcdDriverWriteRamPages(this, LCD_BUFFER_LENGTH);
            vTaskDelay(100);

            break;
        }
        vTaskDelay(1);
    }
}

static void LcdStartupHandle(void) {
    Byte* bytes = tpmsApp.lcdDriver.dataBuff;

    bytes[4].bits.b0 = 1;       // Celcius 1st icon
    // bytes[4].bits.b1 = 1;       // Fahrenheit 1st icon
    // bytes[4].bits.b2 = 1;       // Fahrenheit 2st icon
    bytes[4].bits.b3 = 1;       // Celcius 2nd icon

    // bytes[4].bits.b4 = 1;
    // bytes[4].bits.b5 = 1;
    // bytes[4].bits.b6 = 1;
    // bytes[4].bits.b7 = 1;

    bytes[12].bits.b0 = 1;      // Car Frame
    bytes[12].bits.b1 = 1;      // Car Power Frame
    bytes[12].bits.b3 = 1;      // Backup tire

    bytes[23].bits.b0 = 1;      // BAR 1st icon
    // bytes[23].bits.b1 = 1;      // PSI 1st icon
    // bytes[23].bits.b2 = 1;      // PSI 2nd icon
    bytes[23].bits.b3 = 1;      // BAR 2nd icon

    // bytes[23].bits.b4 = 1;
    // bytes[23].bits.b5 = 1;
    // bytes[23].bits.b6 = 1;
    // bytes[23].bits.b7 = 1;
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
    for (uint8_t i = 0; i < digitNum; i++) {
        uint8_t byteShift = i * 2;
        bytes[2 - byteShift].bits.b5 = digits[i].e;
        bytes[2 - byteShift].bits.b6 = digits[i].g;
        bytes[2 - byteShift].bits.b7 = digits[i].f;
        bytes[3 - byteShift].bits.b4 = digits[i].d;
        bytes[3 - byteShift].bits.b5 = digits[i].c;
        bytes[3 - byteShift].bits.b6 = digits[i].b;
        bytes[3 - byteShift].bits.b7 = digits[i].a;
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
    for (uint8_t i = 0; i < digitNum; i++) {
        uint8_t byteShift = i * 2;
        bytes[21 - byteShift].bits.b0 = digits[i].f;
        bytes[21 - byteShift].bits.b1 = digits[i].g;
        bytes[21 - byteShift].bits.b2 = digits[i].e;
        bytes[22 - byteShift].bits.b0 = digits[i].a;
        bytes[22 - byteShift].bits.b1 = digits[i].b;
        bytes[22 - byteShift].bits.b2 = digits[i].c;
        bytes[22 - byteShift].bits.b3 = digits[i].d;
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
    for (uint8_t i = 0; i < digitNum; i++) {
        uint8_t byteShift = i * 2;
        bytes[8 - byteShift].bits.b4 = digits[i].d;
        bytes[8 - byteShift].bits.b5 = digits[i].c;
        bytes[8 - byteShift].bits.b6 = digits[i].b;
        bytes[8 - byteShift].bits.b7 = digits[i].a;
        bytes[7 - byteShift].bits.b5 = digits[i].e;
        bytes[7 - byteShift].bits.b6 = digits[i].g;
        bytes[7 - byteShift].bits.b7 = digits[i].f;
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
        uint8_t byteShift = i * 2;
        bytes[17 - byteShift].bits.b5 = digits[i].e;
        bytes[17 - byteShift].bits.b6 = digits[i].g;
        bytes[17 - byteShift].bits.b7 = digits[i].f;
        bytes[18 - byteShift].bits.b4 = digits[i].d;
        bytes[18 - byteShift].bits.b5 = digits[i].c;
        bytes[18 - byteShift].bits.b6 = digits[i].b;
        bytes[18 - byteShift].bits.b7 = digits[i].a;
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

    SegmentUpdateDigits(digits, val, digitNum, minDigitShow);
    for (uint8_t i = 0; i < digitNum; i++) {
        uint8_t byteShift = i * 2;
        bytes[0 + byteShift].bits.b0 = digits[i].a;
        bytes[0 + byteShift].bits.b1 = digits[i].b;
        bytes[0 + byteShift].bits.b2 = digits[i].c;
        bytes[0 + byteShift].bits.b3 = digits[i].d;
        bytes[1 + byteShift].bits.b1 = digits[i].f;
        bytes[1 + byteShift].bits.b2 = digits[i].g;
        bytes[1 + byteShift].bits.b3 = digits[i].e;
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

    SegmentUpdateDigits(digits, val, digitNum, minDigitShow);
    for (uint8_t i = 0; i < digitNum; i++) {
        uint8_t byteShift = i * 2;
        bytes[19 + byteShift].bits.b4 = digits[i].a;
        bytes[19 + byteShift].bits.b5 = digits[i].b;
        bytes[19 + byteShift].bits.b6 = digits[i].c;
        bytes[19 + byteShift].bits.b7 = digits[i].d;
        bytes[20 + byteShift].bits.b5 = digits[i].f;
        bytes[20 + byteShift].bits.b6 = digits[i].g;
        bytes[20 + byteShift].bits.b7 = digits[i].e;
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

    SegmentUpdateDigits(digits, val, digitNum, minDigitShow);
    for (uint8_t i = 0; i < digitNum; i++) {
        uint8_t byteShift = i * 2;
        bytes[5 + byteShift].bits.b0 = digits[i].a;
        bytes[5 + byteShift].bits.b1 = digits[i].b;
        bytes[5 + byteShift].bits.b2 = digits[i].c;
        bytes[5 + byteShift].bits.b3 = digits[i].d;
        bytes[6 + byteShift].bits.b1 = digits[i].f;
        bytes[6 + byteShift].bits.b2 = digits[i].g;
        bytes[6 + byteShift].bits.b3 = digits[i].e;
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

    SegmentUpdateDigits(digits, val, digitNum, minDigitShow);
    for (uint8_t i = 0; i < digitNum; i++) {
        uint8_t byteShift = i * 2;
        bytes[15 + byteShift].bits.b0 = digits[i].a;
        bytes[15 + byteShift].bits.b1 = digits[i].b;
        bytes[15 + byteShift].bits.b2 = digits[i].c;
        bytes[15 + byteShift].bits.b3 = digits[i].d;
        bytes[16 + byteShift].bits.b1 = digits[i].f;
        bytes[16 + byteShift].bits.b2 = digits[i].g;
        bytes[16 + byteShift].bits.b3 = digits[i].e;
    }
}

static void powerSource_writeSegmentHandle(SegElement* this, int32_t val) {
    (void)this;

    Byte* bytes = tpmsApp.lcdDriver.dataBuff;
    const uint8_t digitNum = 2;
    const uint8_t minDigitShow = 2;
    Digit digits[digitNum];

    bytes[12].bits.b2 = 1; // Floating point
    if (val >= 100) {
        bytes[10].bits.b3 = 1;
    }
    else {
        bytes[10].bits.b3 = 0;
    }

    SegmentUpdateDigits(digits, val, digitNum, minDigitShow);
    for (uint8_t i = 0; i < digitNum + 1; i++) {
        uint8_t byteShift = i * 3;
        bytes[14 - byteShift].bits.b0 = digits[i].a;
        bytes[14 - byteShift].bits.b1 = digits[i].b;
        bytes[14 - byteShift].bits.b2 = digits[i].c;
        bytes[14 - byteShift].bits.b3 = digits[i].d;
        bytes[13 - byteShift].bits.b0 = digits[i].f;
        bytes[13 - byteShift].bits.b1 = digits[i].g;
        bytes[13 - byteShift].bits.b2 = digits[i].e;
    }
}
