/*
 * display_app.c
 *
 *  Created on: Mar 3, 2023
 *      Author: khanh
 */

#include "app.h"

static void tire0_writeSegmentHandle(SegElement* this, int32_t val);
static void tire1_writeSegmentHandle(SegElement* this, int32_t val);
static void tire2_writeSegmentHandle(SegElement* this, int32_t val);
static void tire3_writeSegmentHandle(SegElement* this, int32_t val);



void DisplayAppInit(void) {
	tpmsApp.lcdDriver.state = LCD_ST_SETUP;
	LcdDriverInit(&tpmsApp.lcdDriver, I2cHwSend);
}

void LcdConfigSettings(void) {
	LcdDriverConfigSettings(&tpmsApp.lcdDriver, 0x00);
	tpmsApp.lcdDriver.state = LCD_ST_ACTIVE;
}

void LcdUpdateData(void) {
	memset(tpmsApp.lcdDriver.dataBuff, 0xff, 12);
	LcdDriverWriteRamPages(&tpmsApp.lcdDriver, 16);
	vTaskDelay(1000);

	memset(tpmsApp.lcdDriver.dataBuff, 0x00, 12);
	LcdDriverWriteRamPages(&tpmsApp.lcdDriver, 16);
	vTaskDelay(1000);

	for (uint8_t i = 0; i < 100; i++) {
		tire0_writeSegmentHandle(NULL, i);
		tire1_writeSegmentHandle(NULL, i);
		tire2_writeSegmentHandle(NULL, i);
		tire3_writeSegmentHandle(NULL, i);
		LcdDriverWriteRamPages(&tpmsApp.lcdDriver, 16);
		vTaskDelay(200);
	}

}

void DisplayTask(void* pv) {
	(void)pv;

	DisplayAppInit();
	I2cHwConfig();

	while (1) {
		switch (tpmsApp.lcdDriver.state) {
		case LCD_ST_SETUP:
			LcdConfigSettings();
			break;
		case LCD_ST_STARTUP:
			break;
		case LCD_ST_IDLE:
			break;
		case LCD_ST_ACTIVE:
			LcdUpdateData();
			break;
		}
		vTaskDelay(100);
	}
}

static void tire0_writeSegmentHandle(SegElement* this, int32_t val) {
	(void)this;

	Byte* bytes = tpmsApp.lcdDriver.dataBuff;
	const uint8_t digitNum = 3;
	const uint8_t minDigitShow = 2;
	Digit digits[digitNum];

	bytes[5].bits.b0 = 1;
	SegmentUpdateDigits(digits, val, digitNum, minDigitShow);
	for (uint8_t i = 0; i < minDigitShow;i++) {
		bytes[5 - i].bits.b1 = digits[i].e;
		bytes[5 - i].bits.b2 = digits[i].g;
		bytes[5 - i].bits.b3 = digits[i].f;
		bytes[5 - i].bits.b4 = digits[i].d;
		bytes[5 - i].bits.b5 = digits[i].c;
		bytes[5 - i].bits.b6 = digits[i].b;
		bytes[5 - i].bits.b7 = digits[i].a;
	}
}

static void tire1_writeSegmentHandle(SegElement* this, int32_t val) {
	(void)this;

	Byte* bytes = tpmsApp.lcdDriver.dataBuff;
	const uint8_t digitNum = 3;
	const uint8_t minDigitShow = 2;
	Digit digits[digitNum];

	bytes[7].bits.b0 = 1;
	SegmentUpdateDigits(digits, val, digitNum, minDigitShow);
	for (uint8_t i = 0; i < minDigitShow;i++) {
		bytes[7 - i].bits.b1 = digits[i].e;
		bytes[7 - i].bits.b2 = digits[i].g;
		bytes[7 - i].bits.b3 = digits[i].f;
		bytes[7 - i].bits.b4 = digits[i].d;
		bytes[7 - i].bits.b5 = digits[i].c;
		bytes[7 - i].bits.b6 = digits[i].b;
		bytes[7 - i].bits.b7 = digits[i].a;
	}
}

static void tire2_writeSegmentHandle(SegElement* this, int32_t val) {
	(void)this;

	Byte* bytes = tpmsApp.lcdDriver.dataBuff;
	const uint8_t digitNum = 3;
	const uint8_t minDigitShow = 2;
	Digit digits[digitNum];

	bytes[10].bits.b4 = 1;
	SegmentUpdateDigits(digits, val, digitNum, minDigitShow);
	for (uint8_t i = 0; i < minDigitShow;i++) {
		bytes[10 + i].bits.b0 = digits[i].d;
		bytes[10 + i].bits.b1 = digits[i].c;
		bytes[10 + i].bits.b2 = digits[i].b;
		bytes[10 + i].bits.b3 = digits[i].a;
		bytes[10 + i].bits.b5 = digits[i].e;
		bytes[10 + i].bits.b6 = digits[i].g;
		bytes[10 + i].bits.b7 = digits[i].f;
	}
}

static void tire3_writeSegmentHandle(SegElement* this, int32_t val) {
	(void)this;

	Byte* bytes = tpmsApp.lcdDriver.dataBuff;
	const uint8_t digitNum = 3;
	const uint8_t minDigitShow = 2;
	Digit digits[digitNum];

	bytes[12].bits.b4 = 1;
	SegmentUpdateDigits(digits, val, digitNum, minDigitShow);
	for (uint8_t i = 0; i < minDigitShow;i++) {
		bytes[12 + i].bits.b0 = digits[i].d;
		bytes[12 + i].bits.b1 = digits[i].c;
		bytes[12 + i].bits.b2 = digits[i].b;
		bytes[12 + i].bits.b3 = digits[i].a;
		bytes[12 + i].bits.b5 = digits[i].e;
		bytes[12 + i].bits.b6 = digits[i].g;
		bytes[12 + i].bits.b7 = digits[i].f;
	}
}

