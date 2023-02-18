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

typedef struct TpmsApp_t TpmsApp;
struct TpmsApp_t {
    MqttClient mqtt;
    Ec200u lteModule;
};

extern TpmsApp tpmsApp;

void MqttAppInit(TpmsApp* this);

#endif /* APP_APP_H_ */
