/**
****************************************************************************************
* @file       atk_denesp32s3_key.c
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


#include "atk_denesp32s3_key_cfg.h"
#include "atk_denesp32s3_key.h"

#include <stdio.h>
#include <stdlib.h>

#include "esp_log.h"
#include "esp_err.h"
#include "driver/gpio.h"
/*
 * DEFINES
 *****************************************************************************************
 */
 
#define ATK_DENESP32S3_KEY1_NUM GPIO_NUM_0

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


/*
 * LOCAL FUNCTION DEFINITIONS
 *****************************************************************************************
 */
 static void _key_init(void *const me)
 {
    printf("atk_denesp32s3_key_init\n");
    atk_denesp32s3_key_t *const me_ = (atk_denesp32s3_key_t *const)me;

    gpio_config_t io_conf = {
        .intr_type = GPIO_INTR_DISABLE,
        .mode = GPIO_MODE_INPUT,
        .pin_bit_mask = 1ULL << me_->atk_denesp32s3_key_num,
        .pull_down_en = GPIO_PULLDOWN_DISABLE,
        .pull_up_en = GPIO_PULLUP_ENABLE,
    };

    ESP_ERROR_CHECK(gpio_config(&io_conf));
 }
 static uint8_t _key_read_state(void *const me,const uint32_t key_id)
 {

    atk_denesp32s3_key_t *const me_ = (atk_denesp32s3_key_t *const)me;
    //key_dev_t *p_dev = &me_->atk_denesp32s3_key_dev;

    int key_state = 0;
  
    key_state = gpio_get_level(me_->atk_denesp32s3_key_num);

    // printf("atk_denesp32s3_key_read_state:[%ld]:%d\n",key_id,key_state);


    return (uint8_t )key_state;
 }

/*
 * GLOBAL FUNCTION DEFINITIONS
 *****************************************************************************************
 */


void atk_denesp32s3_key_ctor(atk_denesp32s3_key_t *const me,const uint32_t key_id,const uint32_t key_num)
{
    static const key_dev_inf_t atk_denesp32s3_key_inf =
    {
        _key_init,
        _key_read_state,
    };

    key_dev_t *p_dev = &me->atk_denesp32s3_key_dev;

    key_dev_ctor(p_dev);

    p_dev->p_inf = &atk_denesp32s3_key_inf;
    p_dev->p_param = me;
    p_dev->key_id = key_id;

    me->atk_denesp32s3_key_num = key_num;
    //   key_dev_register(p_dev);
}










/********************************************* END OF FILE *********************************************/
