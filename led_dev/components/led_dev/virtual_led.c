
#include "virtual_led.h"


#include <stdio.h>
#include <stdlib.h>


#include "esp_log.h"
#include "esp_err.h"

static void _led_init(void *const me)
{
    printf("virtual led init\r\n");
}
static void _led_set(void *const me, const uint8_t status)
{
    ESP_ERROR_CHECK((me != NULL?ESP_OK:ESP_FAIL));

    virtual_led_t *const self = (virtual_led_t *const)me;
    self->virtual_led_status = status;
    printf("virtual led set:%d\r\n",status);
}
static void _led_toggle(void *const me)
{
    ESP_ERROR_CHECK((me != NULL?ESP_OK:ESP_FAIL));

    virtual_led_t *const self = (virtual_led_t *const)me;
    self->virtual_led_status = !self->virtual_led_status ;
    printf("virtual led toggle:%d\r\n",self->virtual_led_status);
}


void virtual_led_ctor(virtual_led_t *const me)
{
    ESP_ERROR_CHECK((me != NULL?ESP_OK:ESP_FAIL));


    static const led_dev_inf_t _led_dev_inf = {
        _led_init,
        _led_set,
        _led_toggle
    };

    lb_led_dev_ctor((led_dev_t *const)&me->virtual_led_dev);

    me->virtual_led_dev.p_led_dev_inf = &_led_dev_inf;
    me->virtual_led_dev.p_arg = me;
    me->virtual_led_status = 0;
}

