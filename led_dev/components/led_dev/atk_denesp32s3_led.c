
#include "atk_denesp32s3.h"


#include <stdio.h>
#include <stdlib.h>


#include "esp_log.h"
#include "esp_err.h"



#define ATK_DEINESP32S3_LED_NUM 1




static void _led_init(void *const me)
{
    gpio_config_t io_conf ={
        .intr_type = GPIO_INTR_DISABLE,
        .mode = GPIO_MODE_INPUT_OUTPUT,
        .pin_bit_mask = GPIO_SEL_2,
        .pull_down_en = 0,
        .pull_up_en = 0,
    };
    esp_err_t gpio_config(const gpio_config_t *pGPIOConfig)
}
static void _led_set(void *const me, const uint8_t status)
{
    ESP_ERROR_CHECK((me != NULL?ESP_OK:ESP_FAIL));

    atk_denesp32s3_led_t *const self = (atk_denesp32s3_led_t *const)me;
    self->atk_denesp32s3_led_status = status;
    printf("virtual led set:%d\r\n",status);
}
static void _led_toggle(void *const me)
{
    ESP_ERROR_CHECK((me != NULL?ESP_OK:ESP_FAIL));

    atk_denesp32s3_led_t *const self = (atk_denesp32s3_led_t *const)me;
    self->atk_denesp32s3_led_status = !self->atk_denesp32s3_led_status ;
    printf("virtual led toggle:%d\r\n",self->atk_denesp32s3_led_status);
}


void atk_denesp32s3_led_ctor(atk_denesp32s3_led_t *const me)
{
    ESP_ERROR_CHECK((me != NULL?ESP_OK:ESP_FAIL));


    static const led_dev_inf_t _led_dev_inf = {
        _led_init,
        _led_set,
        _led_toggle
    };

    lb_led_dev_ctor((led_dev_t *const)&me->atk_denesp32s3_led_dev);

    me->atk_denesp32s3_led_dev.p_led_dev_inf = &_led_dev_inf;
    me->atk_denesp32s3_led_dev.p_arg = me;
    me->atk_denesp32s3_led_status = 0;
}

