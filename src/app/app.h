/*
 * app.h
 *
 *  Created on: Feb 17, 2023
 *      Author: khanh
 */

#ifndef APP_APP_H_
#define APP_APP_H_

#include "board.h"
#include "mqtt_client.h"
#include "ec200u.h"
#include "ht16c23.h"
#include "tire.h"

#include "display_app.h"
#include "mqtt_app.h"
#include "task_init.h"

typedef struct TpmsApp_t TpmsApp;
struct TpmsApp_t {
    MqttClient mqtt;
    Ec200u lteModule;
    uint32_t timestamp;
    LcdDriver lcdDriver;
    Tire tires[4];
};

extern TpmsApp tpmsApp;

#endif /* APP_APP_H_ */
