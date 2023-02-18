/*
 * mqtt_app.c
 *
 *  Created on: Feb 17, 2023
 *      Author: khanh
 */

#include "app.h"
#include "mqtt_config.h"
#include "uart_hw.h"

static void MqttClientInitHandle(MqttClient* this);
static void MqttClientOpenNetworkHandle(MqttClient* this);
static void MqttClientConnectServerHandle(MqttClient* this);
static void MqttClientPublishMessageHandle(MqttClient* this);

Ec200u* pLte = &tpmsApp.lteModule;

void ec200uHwReceiveHandle(UartHw* this) {
    if (this->rxBuff[this->rxIndex - 1] != '\n') return;
    memcpy(tpmsApp.lteModule.base.rxBuff, this->rxBuff, strlen(this->rxBuff));
    tpmsApp.lteModule.base.isResponse = true;

}

void lteAtCmdSend(const char* msg) {
    UartHwSend(&ec200uHw, msg);
}

void MqttAppInit(TpmsApp* this) {
    ec200uHw.receive_handle = ec200uHwReceiveHandle;

    this->lteModule.base.sendCmd = lteAtCmdSend;

    this->mqtt.init = MqttClientInitHandle;
    this->mqtt.openNetwork = MqttClientOpenNetworkHandle;
    this->mqtt.connectServer = MqttClientConnectServerHandle;
    this->mqtt.publishMessage = MqttClientPublishMessageHandle;

    Ec200uInit(&this->lteModule);
}

static void MqttClientInitHandle(MqttClient* this) {
    bool isSuccess = Ec200uCheckATCmd(pLte);
    if (isSuccess) {
        MqttClientSetState(this, MQTT_CLIENT_ST_OPEN_NETWORK);
    }
}

static void MqttClientOpenNetworkHandle(MqttClient* this) {
    bool isSuccess = Ec200uOpenNetwork(pLte, MQTT_NETWORK_HOST, MQTT_NETWORK_PORT);
    if (isSuccess) {
        MqttClientSetState(this, MQTT_CLIENT_ST_CONNECT_SERVER);
    }
}

static void MqttClientConnectServerHandle(MqttClient* this) {
    bool isSuccess = Ec200uConnectServer(pLte, "TPMS_01", MQTT_CLIENT_USERNAME, MQTT_CLIENT_PASSWORD);
    if (isSuccess) {
        MqttClientSetState(this, MQTT_CLIENT_ST_STREAM_DATA);
    }
}

static void MqttClientPublishMessageHandle(MqttClient* this) {
    char topic[32] = { 0 };
    strcat(topic, MQTT_TOPIC_ROOT_PATH);
    strcat(topic, "TPMS_01");
    strcat(topic, MQTT_TOPIC_SENSOR_ENDPOINT);
    bool isSuccess = Ec200uPublishMessage(pLte, MQTT_CONFIG_QOS, MQTT_CONFIG_RETAIN, topic, "hello");
    if (isSuccess) {
        MqttClientSetState(this, MQTT_CLIENT_ST_STREAM_DATA);
    }
}
