#pragma once

#include <stdint.h>


typedef struct _led_dev_inf_t
{
    void (*_led_init)(void *const me);
    void (*_led_set)(void *const me, const uint8_t status);
    void (*_led_toggle)(void *const me);
} led_dev_inf_t;

typedef struct _led_dev_t
{
    const led_dev_inf_t * p_led_dev_inf;

    void *p_arg;
} led_dev_t;




static inline void led_dev_init(led_dev_t *const me)
{
    if(!me && !me->p_led_dev_inf->_led_init)
    {
        me->p_led_dev_inf->_led_init(me->p_arg);
    }
}

static inline void led_dev_led_set(led_dev_t *const me,const uint8_t status)
{
    if(!me && !me->p_led_dev_inf->_led_set)
    {
        me->p_led_dev_inf->_led_set(me->p_arg,status);
    }
}

static inline void led_dev_led_toggle(led_dev_t *const me)
{
    if(!me && !me->p_led_dev_inf->_led_toggle)
    {
        me->p_led_dev_inf->_led_toggle(me->p_arg);
    }
}

void led_dev_ctor(led_dev_t *const me);