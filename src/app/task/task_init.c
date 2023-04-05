#include "app.h"


static void SystemHandleTask(void* pv);

void TaskInit(void) {
    /* Create the queue. */
    QueueHandle_t xQueue = NULL;
    xQueue = xQueueCreate(5, sizeof(unsigned long));

    if (xQueue != NULL)
    {
        xTaskCreate(SystemHandleTask, "System", configMINIMAL_STACK_SIZE, NULL, configMAX_PRIORITIES - 4U, NULL);
        xTaskCreate(MqttHandleTask, "MQTT", 256, NULL, configMAX_PRIORITIES - 5U, NULL);
        xTaskCreate(HandleSensorAppTask, "Sensor", 512, NULL, configMAX_PRIORITIES - 5U, NULL);
        xTaskCreate(DisplayTask, "Display", 256, NULL, configMAX_PRIORITIES - 5U, NULL);

        vTaskStartScheduler();
    }
}

static void SystemHandleTask(void* pv) {
    (void)pv;

    while (1) {
        tpmsApp.timestamp++;
        ClockProcessTimeout(&tpmsApp.lteModule.base.clock);
        vTaskDelay(1);
    }
}

