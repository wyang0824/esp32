
#include "atk_denesp32s3_led.h"


#include <stdio.h>
#include <stdlib.h>


#include "esp_log.h"
#include "esp_err.h"
#include "driver/gpio.h"


#define ATK_DEINESP32S3_LED_NUM GPIO_NUM_1




static void _led_init(void *const me)
{
    const gpio_config_t io_conf ={
        .intr_type = GPIO_INTR_DISABLE,
        .mode = GPIO_MODE_INPUT_OUTPUT,
        .pin_bit_mask = 1 << ATK_DEINESP32S3_LED_NUM,
        .pull_down_en = GPIO_PULLDOWN_DISABLE,
        .pull_up_en = GPIO_PULLUP_ENABLE,
    };
    ESP_ERROR_CHECK(gpio_config(&io_conf));

    printf("atk_denesp32s3 led init\r\n");
}
static void _led_set(void *const me, const uint8_t status)
{
    ESP_ERROR_CHECK((me != NULL?ESP_OK:ESP_FAIL));

    atk_denesp32s3_led_t *const self = (atk_denesp32s3_led_t *const)me;

    self->atk_denesp32s3_led_status = status;
    printf("atk_denesp32s3 led set:%d\r\n",status);
    ESP_ERROR_CHECK(gpio_set_level(ATK_DEINESP32S3_LED_NUM,self->atk_denesp32s3_led_status));
}
static void _led_toggle(void *const me)
{
    ESP_ERROR_CHECK((me != NULL?ESP_OK:ESP_FAIL));

    atk_denesp32s3_led_t *const self = (atk_denesp32s3_led_t *const)me;
    self->atk_denesp32s3_led_status = !self->atk_denesp32s3_led_status ;
    printf("virtual led toggle:%d\r\n",self->atk_denesp32s3_led_status);
    ESP_ERROR_CHECK(gpio_set_level(ATK_DEINESP32S3_LED_NUM,self->atk_denesp32s3_led_status));
}


void atk_denesp32s3_led_ctor(atk_denesp32s3_led_t *const me)
{
    ESP_ERROR_CHECK((me != NULL?ESP_OK:ESP_FAIL));


    static const led_dev_inf_t _led_dev_inf = {
        _led_init,
        _led_set,
        _led_toggle
    };

    led_dev_ctor((led_dev_t *const)&me->atk_denesp32s3_led_dev);

    me->atk_denesp32s3_led_dev.p_led_dev_inf = &_led_dev_inf;
    me->atk_denesp32s3_led_dev.p_arg = me;
    me->atk_denesp32s3_led_status = 0;
}

