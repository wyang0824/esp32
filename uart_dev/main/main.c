#include <stdio.h>

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"


#include "uart.h"

void app_main(void)
{
    uart_ctor();


    while(1)
    {
        //printf("app_main\n");
        vTaskDelay(1000 / portTICK_PERIOD_MS);
    }
}
