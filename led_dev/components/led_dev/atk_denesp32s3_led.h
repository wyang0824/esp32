#pragma once


#include "led_dev.h"




typedef struct _atk_denesp32s3_led_t
{
    led_dev_t   atk_denesp32s3_led_dev;
    uint8_t     atk_denesp32s3_led_status;
} atk_denesp32s3_led_t;


void atk_denesp32s3_led_ctor(atk_denesp32s3_led_t *const me);