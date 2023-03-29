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
#include "gps.h"

#include "mqtt_app.h"
#include "sensor_app.h"
#include "task_init.h"
#include "tire.h"

#define TIRE_NUMBER                         4

typedef struct TpmsApp_t TpmsApp;
struct TpmsApp_t {
    MqttClient mqtt;
    Gps gps;
    Ec200u lteModule;
    Tire tires[TIRE_NUMBER];
    uint32_t timestamp;
};

extern TpmsApp tpmsApp;

#endif /* APP_APP_H_ */
