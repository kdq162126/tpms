/*
 * mqtt_app.c
 *
 *  Created on: Feb 17, 2023
 *      Author: khanh
 */

#include "app.h"
#include "mqtt_config.h"
#include "stdlib.h"
#include "stdio.h"

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

    // FIXME:
    srand(100);
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
void CreateMockSensorMessage(char* buff, char* deviceId) {
    buff = JsonOpen(buff);
    buff = JsonFromString(buff, "Id", deviceId);
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
        buff = JsonFromString(buff, "id", tireIds[i]);
        *buff++ = ',';
        // buff = JsonFromString(buff, "tireName", tireNames[i]);
        // *buff++ = ',';
        buff = JsonFromString(buff, "pos", positions[i]);
        *buff++ = ',';
        buff = JsonFromInt(buff, "pres", rand()%20);
        *buff++ = ',';
        buff = JsonFromInt(buff, "bat", rand()%100);
        *buff++ = ',';
        buff = JsonFromInt(buff, "temp", rand()%50);
        buff = JsonClose(buff);
        *buff++ = ',';
    }
    buff--;
    *buff++ = ']';
    buff = JsonClose(buff);
    *buff++ = '\0';
}

void CreateMockLocationMessage(char* buff, char* deviceId) {
    buff = JsonOpen(buff);
    buff = JsonFromString(buff, "id", deviceId);
    *buff++ = ',';
    // buff = JsonFromString(buff, "version", "123xxx");
    // *buff++ = ',';
    buff = JsonFromFloat(buff, "lat", 21.00+(float)(rand() % 10000) / 1000000, 6);
    *buff++ = ',';
    buff = JsonFromFloat(buff, "lon", 105.81 + (float)(rand() % 10000) / 1000000, 6);
    buff = JsonClose(buff);
    *buff++ = '\0';
}

char* deviceIds[] = {"TPMS001", "TPMS002"};
char* TopicSensors[] = {
		"/device/TPMS001/sensor",
		"/device/TPMS002/sensor",
};

char* TopicLocations[] = {
		"/device/TPMS001/location",
		"/device/TPMS002/location",
};



static void MqttClientPublishMessageHandle(MqttClient* this) {
	for (uint8_t i = 0; i < 2; i++) {
	    CreateMockSensorMessage(tpmsApp.lteModule.base.txDataBuff, deviceIds[i]);
	    bool isSuccess = Ec200uPublishMessage(&pApp->lteModule, MQTT_CONFIG_QOS, MQTT_CONFIG_RETAIN, TopicSensors[i], tpmsApp.lteModule.base.txDataBuff);
	    if (!isSuccess) {
	        MqttClientSetState(this, MQTT_CLIENT_ST_FAIL);
	        return;
	    }

	    tpmsApp.lteModule.base.delayMs(1000);

	    CreateMockLocationMessage(tpmsApp.lteModule.base.txDataBuff, deviceIds[i]);
	    isSuccess = Ec200uPublishMessage(&pApp->lteModule, MQTT_CONFIG_QOS, MQTT_CONFIG_RETAIN, TopicLocations[i], tpmsApp.lteModule.base.txDataBuff);
	    if (!isSuccess) {
	        MqttClientSetState(this, MQTT_CLIENT_ST_FAIL);
	        return;
	    }

	    MqttClientSetState(this, MQTT_CLIENT_ST_STREAM_DATA);

	    tpmsApp.lteModule.base.delayMs(1000);
	}
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


