#include "app.h"

int main(void) {
    SystemClockConfig();
    SystemPinsConfig();

    UartHwInit();
    MqttAppInit(&tpmsApp);

    while (1) {
        switch (tpmsApp.mqtt.state) {
        case MQTT_CLIENT_ST_INIT:
            MqttClientInit(&tpmsApp.mqtt);
            break;
        case MQTT_CLIENT_ST_OPEN_NETWORK:
            MqttClientOpenNetwork(&tpmsApp.mqtt);
            break;
        case MQTT_CLIENT_ST_CONNECT_SERVER:
            MqttClientConnectServer(&tpmsApp.mqtt);
            break;
        case MQTT_CLIENT_ST_STREAM_DATA:
            MqttClientPublishMessage(&tpmsApp.mqtt);
            break;
        }
    }
}
