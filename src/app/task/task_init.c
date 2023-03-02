#include "app.h"


static void SystemHandleTask(void* pv);
static void MqttHandleTask(void* pv);

void TaskInit() {
    /* Create the queue. */
    QueueHandle_t xQueue = NULL;
    xQueue = xQueueCreate(1, sizeof(unsigned long));

    if (xQueue != NULL)
    {
        xTaskCreate(SystemHandleTask, "System", configMINIMAL_STACK_SIZE, NULL, configMAX_PRIORITIES - 2U, NULL);
        xTaskCreate(MqttHandleTask, "MQTT", configMINIMAL_STACK_SIZE, NULL, configMAX_PRIORITIES - 3U, NULL);

        vTaskStartScheduler();
    }
}

static void SystemHandleTask(void* pv) {

    while (1) {
        tpmsApp.timestamp++;
        ClockProcessTimeout(&tpmsApp.lteModule.base.clock);
        vTaskDelay(1);
    }
}

static void MqttHandleTask(void* pv) {
    (void)pv;

    MqttAppInit();
    UartHwConfig(&ec200uHw);
    vTaskDelay(1000);

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
            vTaskDelay(10000);
            break;
        case MQTT_CLIENT_ST_FAIL:
            MqttClientFailHandle(&tpmsApp.mqtt);
            // vTaskDelay(15000);
            break;
        }

        vTaskDelay(1);
    }
}
