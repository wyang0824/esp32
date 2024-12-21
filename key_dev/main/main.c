#include <stdio.h>

#include "key.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"


void app_main(void)
{
    key_ctor();

    while(1)
    {
        // key_handler();
        vTaskDelay(1000 / portTICK_PERIOD_MS);
    }
}
