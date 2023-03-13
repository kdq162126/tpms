#include "app.h"


static void SystemHandleTask(void* pv);

void TaskInit(void) {
    /* Create the queue. */
    QueueHandle_t xQueue = NULL;
    xQueue = xQueueCreate(1, sizeof(unsigned long));

    if (xQueue != NULL)
    {
        xTaskCreate(SystemHandleTask, "System", configMINIMAL_STACK_SIZE, NULL, configMAX_PRIORITIES - 2U, NULL);
        // xTaskCreate(MqttHandleTask, "MQTT", configMINIMAL_STACK_SIZE, NULL, configMAX_PRIORITIES - 3U, NULL);
        xTaskCreate(DisplayTask, "Display", configMINIMAL_STACK_SIZE, NULL, configMAX_PRIORITIES - 3U, NULL);

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




