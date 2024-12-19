#include <stdio.h>
#include "led_dev.h"
#include "virtual_led.h"
#include "atk_denesp32s3_led.h"

//对于FreeRTOS 
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

static led_dev_t s_tled_dev_t;

static virtual_led_t s_tvirtual_led;
static atk_denesp32s3_led_t s_tatk_denesp32s3_led;

void app_main(void)
{
    led_dev_ctor(&s_tled_dev_t);
    led_dev_init(&s_tled_dev_t);

    virtual_led_ctor(&s_tvirtual_led);
    led_dev_init((led_dev_t *const )&s_tvirtual_led);   


    atk_denesp32s3_led_ctor(&s_tatk_denesp32s3_led);
    led_dev_init((led_dev_t *const )&s_tatk_denesp32s3_led);    

    while(1)
    {
        led_dev_led_toggle((led_dev_t *const )&s_tvirtual_led);
        led_dev_led_toggle((led_dev_t *const )&s_tatk_denesp32s3_led);

        vTaskDelay(1000);
    }
}
