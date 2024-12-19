#include <stdio.h>
#include "led_dev.h"

static led_dev_t s_tled_dev_t;

void app_main(void)
{
    led_dev_ctor(&s_tled_dev_t);
    led_dev_init(&s_tled_dev_t);


    while(1)
    {
        ;
    }
}
