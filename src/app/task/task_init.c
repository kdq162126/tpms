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

        // if (tpmsApp.lcdDriver.state == LCD_ST_ACTIVE) {
        //     for (uint8_t i = 0; i < TIRE_NUMBER; i++) {
        //         Tire* pTire = &tpmsApp.tires[i];

        //         if (pTire->state != TIRE_ST_ACTIVE) continue;

        //         pTire->inactive_counter++;
        //         if (pTire->inactive_counter >= 210000) {
        //             TireSetState(pTire, TIRE_ST_INACTIVE);
        //             SegmentWrite(&pTire->press, -1);
        //             SegmentWrite(&pTire->temp, -1);
        //         }
        //     }
        // }

        vTaskDelay(1);
    }
}

