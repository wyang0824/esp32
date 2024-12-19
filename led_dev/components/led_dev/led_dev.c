#include <stdio.h>
#include <stdlib.h>

#include "led_dev.h"

#include "esp_log.h"
#include "esp_err.h"


static void _led_dev_init_default(void *const me)
{
    printf("_led_dev_init_default\r\n");
}
static void _led_dev_set_default(void *const me, const uint8_t status)
{
    printf("_led_dev_set_default:%d\r\n",status);
}
static void _led_dev_toggle_default(void *const me)
{
    printf("_led_dev_toggle_default\r\n");
}

 


void led_dev_ctor(led_dev_t *const me)
{
    ESP_ERROR_CHECK((me != NULL?ESP_OK:ESP_FAIL));
    static const led_dev_inf_t led_dev_inf_default = {
        _led_dev_init_default,
        _led_dev_set_default,
        _led_dev_toggle_default
    };

    me->p_led_dev_inf = &led_dev_inf_default;
    me->p_arg = me;
}

