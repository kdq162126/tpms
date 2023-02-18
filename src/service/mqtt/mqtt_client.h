/*
 * mqtt_client.h
 *
 *  Created on: Feb 17, 2023
 *      Author: khanh
 */

#ifndef SERVICE_MQTT_MQTT_CLIENT_H_
#define SERVICE_MQTT_MQTT_CLIENT_H_

typedef enum MQTT_CLIENT_STATE {
    MQTT_CLIENT_ST_INIT = 0,
    MQTT_CLIENT_ST_OPEN_NETWORK,
    MQTT_CLIENT_ST_CONNECT_SERVER,
    MQTT_CLIENT_ST_STREAM_DATA,
    MQTT_CLIENT_ST_FAIL,
} MQTT_CLIENT_STATE;

typedef struct MqttClient_t MqttClient;
struct MqttClient_t {
    MQTT_CLIENT_STATE state;
    void (*init)(MqttClient*);
    void (*openNetwork)(MqttClient*);
    void (*connectServer)(MqttClient*);
    void (*publishMessage)(MqttClient*);
};

static inline void MqttClientSetState(MqttClient* this, MQTT_CLIENT_STATE state) {
    this->state = state;
}

static inline MQTT_CLIENT_STATE MqttClientGetState(MqttClient* this) {
    return this->state;
}

static inline void MqttClientInit(MqttClient* this) {
    this->init(this);
}

static inline void MqttClientOpenNetwork(MqttClient* this) {
    this->openNetwork(this);
}

static inline void MqttClientConnectServer(MqttClient* this) {
    this->connectServer(this);
}

static inline void MqttClientPublishMessage(MqttClient* this) {
    this->publishMessage(this);
}

#endif /* SERVICE_MQTT_MQTT_CLIENT_H_ */
