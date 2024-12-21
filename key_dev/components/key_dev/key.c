/**
****************************************************************************************
* @file       key.c
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


#include "key_cfg.h"
#include "key.h"


#include "key_dev.h"
#include "multi_button.h"
#include "atk_denesp32s3_key.h"


#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

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

static Button s_tbutton[ATK_DENESP32S3_KEY_CNT];
static atk_denesp32s3_key_t s_atk_denesp32s3_key[ATK_DENESP32S3_KEY_CNT];

/*
 * LOCAL FUNCTION DEFINITIONS
 *****************************************************************************************
 */

static uint8_t  _key_read_pin_state(uint8_t button_id_)
{
    return (uint8_t)key_dev_read_state(&s_atk_denesp32s3_key[button_id_].atk_denesp32s3_key_dev,button_id_);
}

static void _btn_press(void*p_arg)
{
    Button *p_button = (Button *)p_arg;
    
    
    switch (get_button_event(p_button))
    {
    case PRESS_DOWN/* constant-expression */:
      /* code */
      printf("PRESS_DOWN\n");
      break;
    
    default:
      break;
    }
}
static void _key_task( void * p_handle)
{
  while(1)
  {
    button_ticks();
    vTaskDelay(10 / portTICK_PERIOD_MS);
  }
}
/*
 * GLOBAL FUNCTION DEFINITIONS
 *****************************************************************************************
 */

void key_ctor(void)
{
    for(uint8_t i = 0; i < ATK_DENESP32S3_KEY_CNT; i++)
    {
        atk_denesp32s3_key_ctor(&s_atk_denesp32s3_key[i],i,0);
        key_dev_register(&s_atk_denesp32s3_key[i].atk_denesp32s3_key_dev);
        key_dev_init(&s_atk_denesp32s3_key[i].atk_denesp32s3_key_dev);


        button_init(&s_tbutton[i],_key_read_pin_state,0,i);

        button_attach(&s_tbutton[i],PRESS_DOWN,_btn_press);

        button_start(&s_tbutton[i]);
    }
    xTaskCreate((TaskFunction_t)_key_task, "button_ticks", 2048, NULL, 10, NULL);

} 

uint8_t key_read_state(uint32_t key_id)
{
    return key_dev_read_state_depend_id(key_id);
}


void key_handler(void)
{
    button_ticks();
}





/********************************************* END OF FILE *********************************************/
