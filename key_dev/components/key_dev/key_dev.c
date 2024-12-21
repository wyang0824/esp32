/**
****************************************************************************************
* @file       key_dev.c
* @author     
* @version    V01
* @date       2024.12.21
* @brief       
  ****************************************************************************************
  */

/*
 * INCLUDE FILES
 *****************************************************************************************
 */


#include "key_dev_cfg.h"
#include "key_dev.h"

#include <stdio.h>
#include <stdlib.h>

/*
 * DEFINES
 *****************************************************************************************
 */
 


/*
 * STRUCT DEFINE
 *****************************************************************************************
 */


/*
 * LOCAL FUNCTION DECLARATION
 *****************************************************************************************
 */


/*
 * LOCAL VARIABLE DEFINITIONS
 *****************************************************************************************
 */

static key_dev_t *s_pkey_dev_head = NULL;
/*
 * LOCAL FUNCTION DEFINITIONS
 *****************************************************************************************
 */

 static void _key_dev_init_default(void *const me)
 {
    printf("key_dev_init_default\n");
 }
 static uint8_t _key_dev_read_state_default(void *const me,const uint32_t key_id)
 {
    printf("key_dev_read_state_default\n");
    return 0;
 }
/*
 * GLOBAL FUNCTION DEFINITIONS
 *****************************************************************************************
 */

/// @brief 
/// @param me 
/// @param p_param 
void key_dev_ctor(key_dev_t *const me)
{
    static const key_dev_inf_t default_key_dev_inf =
    {
        _key_dev_init_default,
        _key_dev_read_state_default
    };
    me->p_inf = &default_key_dev_inf;
    me->p_param = me;
    me->key_id = 0;
    me->p_next = NULL;
}

/// @brief 
/// @param me 
/// @param p_param 
void key_dev_register(key_dev_t *const me)
{
    key_dev_t *p_key_dev = s_pkey_dev_head;
    if (NULL == p_key_dev)
    {
        s_pkey_dev_head = me;
    }
    else
    {
        while (NULL != p_key_dev->p_next)
        {
            p_key_dev = p_key_dev->p_next;
        }
        p_key_dev->p_next = me;
    }
}
/// @brief 
/// @param me 
/// @param p_param 
void key_dev_init(key_dev_t *const me)
{
    if(me && me->p_inf && me->p_inf->key_dev_init)
    {
        me->p_inf->key_dev_init(me->p_param);
    }
}

/// @brief 
/// @param me 
/// @param p_param 
void key_dev_all_init(void)
{
    key_dev_t *p_key_dev = s_pkey_dev_head;
    while (NULL != p_key_dev)
    {
        p_key_dev->p_inf->key_dev_init(p_key_dev->p_param);
        p_key_dev = p_key_dev->p_next;
    }
}

/// @brief 
/// @param me 
/// @param p_param 
uint8_t key_dev_read_state(key_dev_t *const me,const uint32_t key_id)
{
    uint8_t state = UINT8_MAX;
    if(me && me->p_inf && me->p_inf->key_dev_read_state)
    {
        state =  me->p_inf->key_dev_read_state(me->p_param,me->key_id);
    }

    return state;
}
/// @brief 
/// @param me 
/// @param p_param 
uint8_t key_dev_read_state_depend_id(const uint32_t id)
{
    uint8_t state = UINT8_MAX;
    key_dev_t *p_key_dev = s_pkey_dev_head;
    while (NULL != p_key_dev)
    {
        printf("key_dev_read_state_depend_id:[%ld]:%p\n",id,p_key_dev);
        if (id == p_key_dev->key_id)
        {
            if(p_key_dev->p_inf->key_dev_read_state)
                state =  p_key_dev->p_inf->key_dev_read_state(p_key_dev->p_param,p_key_dev->key_id);
            break;
        }
        p_key_dev = p_key_dev->p_next;
    }

    return state;
}
/********************* END OF FILE *********************/
