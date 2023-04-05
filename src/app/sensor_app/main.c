#include "sdk_project_config.h"
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"

#include "sensor_app.h"

int main(void)
{
    /* Create the queue. */
    QueueHandle_t xQueue = NULL;
    xQueue = xQueueCreate(1, sizeof(unsigned long));

    if (xQueue != NULL)
    {
        xTaskCreate(SensorTaskHandle, "Sensor", 1024, NULL, 1, NULL);

        vTaskStartScheduler();
    }
}


