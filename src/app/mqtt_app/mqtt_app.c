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
static void lteAtCmdDelay(const uint32_t timeMs);

static void MqttClientInitHandle(MqttClient* this);
static void MqttClientOpenNetworkHandle(MqttClient* this);
static void MqttClientConnectServerHandle(MqttClient* this);
static void MqttClientPublishMessageHandle(MqttClient* this);
static void MqttClientFailHandleImpl(MqttClient* this);

static char TopicSensor[32] = { 0 };
static char TopicLocation[32] = { 0 };

void MqttAppInit() {
    ATmodemInit((ATmodem*)&pApp->lteModule, lteAtCmdSend, &pApp->timestamp, lteAtCmdDelay);
    pApp->lteModule.base.receiveData = ec200uHw.rxBuff;
    pUartHw->receive_handle = ec200uHwReceiveHandle;

    pApp->mqtt.init = MqttClientInitHandle;
    pApp->mqtt.openNetwork = MqttClientOpenNetworkHandle;
    pApp->mqtt.connectServer = MqttClientConnectServerHandle;
    pApp->mqtt.publishMessage = MqttClientPublishMessageHandle;
    pApp->mqtt.failHandle = MqttClientFailHandleImpl;

    // Init MQTT Topic
    strcat(TopicSensor, MQTT_TOPIC_ROOT_PATH);
    strcat(TopicSensor, DEVICE_ID);
    strcat(TopicSensor, MQTT_TOPIC_SENSOR_ENDPOINT);

    strcat(TopicLocation, MQTT_TOPIC_ROOT_PATH);
    strcat(TopicLocation, DEVICE_ID);
    strcat(TopicLocation, MQTT_TOPIC_LOCATION_ENDPOINT);
}

static void ec200uHwReceiveHandle(UartHw* this) {
    if (strstr(this->rxBuff, pApp->lteModule.base.expectMsg) == NULL)
        return;

    pApp->lteModule.base.isResponse = true;
}

static void lteAtCmdSend(const char* msg) {
    UartHwSendMsg(pUartHw, msg);
}

static void lteAtCmdDelay(const uint32_t timeMs) {
    vTaskDelay(timeMs);
}


static void MqttClientInitHandle(MqttClient* this) {
    bool isSuccess = Ec200uCheckATCmd(&pApp->lteModule) &&
        Ec200uTurnOffEcho(&pApp->lteModule) &&
        Ec200uCheckSimReady(&pApp->lteModule);
    if (!isSuccess) {
        MqttClientSetState(this, MQTT_CLIENT_ST_FAIL);
        return;
    }

    MqttClientSetState(this, MQTT_CLIENT_ST_OPEN_NETWORK);
}

static void MqttClientOpenNetworkHandle(MqttClient* this) {
    int8_t networkStt = Ec200uOpenNetwork(&pApp->lteModule, MQTT_NETWORK_HOST, MQTT_NETWORK_PORT);
    switch (networkStt) {
    case 0:
        MqttClientSetState(this, MQTT_CLIENT_ST_CONNECT_SERVER);
        break;
    case 2:
        MqttClientSetState(this, MQTT_CLIENT_ST_STREAM_DATA);
        break;
    default:
        MqttClientSetState(this, MQTT_CLIENT_ST_FAIL);
        break;
    }
}

static void MqttClientConnectServerHandle(MqttClient* this) {
    bool isSuccess = Ec200uConnectServer(&pApp->lteModule, DEVICE_ID, MQTT_CLIENT_USERNAME, MQTT_CLIENT_PASSWORD);
    if (!isSuccess) {
        MqttClientSetState(this, MQTT_CLIENT_ST_FAIL);
        return;
    }

    MqttClientSetState(this, MQTT_CLIENT_ST_STREAM_DATA);
}

// TEST
void CreateMockSensorMessage(char* buff) {
    buff = JsonOpen(buff);
    buff = JsonFromString(buff, "Id", DEVICE_ID);
    *buff++ = ',';
    // buff = JsonFromString(buff, "version", "123xxx");
    // *buff++ = ',';
    buff = StringAppendStringWithQuote(buff, "tires");
    *buff++ = ':';
    *buff++ = '[';

    char* tireIds[] = { "91HAG1","81O012","HMLK87","HNC01L" };
    char* positions[] = { "L11", "L12", "R11" ,"R12" };
    for (uint8_t i = 0; i < 4; i++) {
        buff = JsonOpen(buff);
        buff = JsonFromString(buff, "Id", tireIds[i]);
        *buff++ = ',';
        // buff = JsonFromString(buff, "tireName", tireNames[i]);
        // *buff++ = ',';
        buff = JsonFromString(buff, "pos", positions[i]);
        *buff++ = ',';
        buff = JsonFromInt(buff, "pres", (i + 1) * 10);
        *buff++ = ',';
        buff = JsonFromInt(buff, "bat", (i + 2) * 10);
        *buff++ = ',';
        buff = JsonFromInt(buff, "temp", (i + 1) * 10);
        buff = JsonClose(buff);
        *buff++ = ',';
    }
    buff--;
    *buff++ = ']';
    buff = JsonClose(buff);
    *buff++ = '\0';
}

void CreateMockLocationMessage(char* buff) {
    buff = JsonOpen(buff);
    buff = JsonFromString(buff, "Id", DEVICE_ID);
    *buff++ = ',';
    // buff = JsonFromString(buff, "version", "123xxx");
    // *buff++ = ',';
    buff = JsonFromFloat(buff, "lat", 21.06211607852294);
    *buff++ = ',';
    buff = JsonFromFloat(buff, "lon", 105.81515284301472);
    buff = JsonClose(buff);
    *buff++ = '\0';
}

static void MqttClientPublishMessageHandle(MqttClient* this) {
    CreateMockSensorMessage(tpmsApp.lteModule.base.txDataBuff);
    bool isSuccess = Ec200uPublishMessage(&pApp->lteModule, MQTT_CONFIG_QOS, MQTT_CONFIG_RETAIN, TopicSensor, tpmsApp.lteModule.base.txDataBuff);
    if (!isSuccess) {
        MqttClientSetState(this, MQTT_CLIENT_ST_FAIL);
        return;
    }

    tpmsApp.lteModule.base.delayMs(1000);

    CreateMockLocationMessage(tpmsApp.lteModule.base.txDataBuff);
    isSuccess = Ec200uPublishMessage(&pApp->lteModule, MQTT_CONFIG_QOS, MQTT_CONFIG_RETAIN, TopicLocation, tpmsApp.lteModule.base.txDataBuff);
    if (!isSuccess) {
        MqttClientSetState(this, MQTT_CLIENT_ST_FAIL);
        return;
    }

    MqttClientSetState(this, MQTT_CLIENT_ST_STREAM_DATA);
}

static void MqttClientFailHandleImpl(MqttClient* this) {
    bool isSuccess = Ec200uDeactiveRF(&pApp->lteModule) && Ec200uActiveRF(&pApp->lteModule);
    if (!isSuccess) {
        UartHwConfig(&ec200uHw);
        vTaskDelay(1000);
        return;
    }

    MqttClientSetState(this, MQTT_CLIENT_ST_INIT);
}


