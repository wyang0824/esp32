#pragma once


#include "led_dev.h"




typedef struct _virtual_led_t
{
    led_dev_t virtual_led_dev;
    uint8_t virtual_led_status;
} virtual_led_t;

void virtual_led_ctor(virtual_led_t *const me);

