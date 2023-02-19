/*
 * mqtt_app.c
 *
 *  Created on: Feb 17, 2023
 *      Author: khanh
 */

#include "app.h"
#include "mqtt_config.h"

 // TODO: Assign source to these pointer here! 
UartHw* pUartHw = &ec200uHw;
TpmsApp* pApp = &tpmsApp;

static void ec200uHwReceiveHandle(UartHw* this);
static void lteAtCmdSend(const char* msg);

static void MqttClientInitHandle(MqttClient* this);
static void MqttClientOpenNetworkHandle(MqttClient* this);
static void MqttClientConnectServerHandle(MqttClient* this);
static void MqttClientPublishMessageHandle(MqttClient* this);

void MqttAppInit() {
    ATmodemInit((ATmodem*)&pApp->lteModule, lteAtCmdSend, &pApp->timestamp);
    pUartHw->receive_handle = ec200uHwReceiveHandle;

    pApp->mqtt.init = MqttClientInitHandle;
    pApp->mqtt.openNetwork = MqttClientOpenNetworkHandle;
    pApp->mqtt.connectServer = MqttClientConnectServerHandle;
    pApp->mqtt.publishMessage = MqttClientPublishMessageHandle;
}

static void ec200uHwReceiveHandle(UartHw* this) {
    if (this->rxBuff[this->rxIndex - 1] != '\n')
        return;

    this->rxBuff[this->rxIndex - 1] = '\0';
    ATmodemDetectNewMessage((ATmodem*)&pApp->lteModule, this->rxBuff);
}

static void lteAtCmdSend(const char* msg) {
    UartHwSendMsg(pUartHw, msg);
}


static void MqttClientInitHandle(MqttClient* this) {
    bool isSuccess = Ec200uCheckATCmd(&pApp->lteModule) && Ec200uTurnOffEcho(&pApp->lteModule) && Ec200uCheckSimReady(&pApp->lteModule);
    if (!isSuccess) {
        MqttClientSetState(this, MQTT_CLIENT_ST_FAIL);
        return;
    }

    MqttClientSetState(this, MQTT_CLIENT_ST_OPEN_NETWORK);
}

static void MqttClientOpenNetworkHandle(MqttClient* this) {
    bool isSuccess = Ec200uOpenNetwork(&pApp->lteModule, MQTT_NETWORK_HOST, MQTT_NETWORK_PORT);
    if (!isSuccess) {
        MqttClientSetState(this, MQTT_CLIENT_ST_FAIL);
        return;
    }

    MqttClientSetState(this, MQTT_CLIENT_ST_CONNECT_SERVER);
}

static void MqttClientConnectServerHandle(MqttClient* this) {
    bool isSuccess = Ec200uConnectServer(&pApp->lteModule, "TPMS_01", MQTT_CLIENT_USERNAME, MQTT_CLIENT_PASSWORD);
    if (!isSuccess) {
        MqttClientSetState(this, MQTT_CLIENT_ST_FAIL);
        return;
    }

    MqttClientSetState(this, MQTT_CLIENT_ST_STREAM_DATA);
}

static void MqttClientPublishMessageHandle(MqttClient* this) {
    char topic[32] = { 0 };
    strcat(topic, MQTT_TOPIC_ROOT_PATH);
    strcat(topic, "TPMS_01");
    strcat(topic, MQTT_TOPIC_SENSOR_ENDPOINT);

    bool isSuccess = Ec200uPublishMessage(&pApp->lteModule, MQTT_CONFIG_QOS, MQTT_CONFIG_RETAIN, topic, "hello");
    if (!isSuccess) {
        MqttClientSetState(this, MQTT_CLIENT_ST_FAIL);
        return;
    }

    MqttClientSetState(this, MQTT_CLIENT_ST_STREAM_DATA);
}
