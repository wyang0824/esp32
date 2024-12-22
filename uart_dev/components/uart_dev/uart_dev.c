/**
****************************************************************************************
* @file       uart_dev.c
* @author     
* @version    V01
* @date       2024.12.22
* @brief       
  ****************************************************************************************
  */

/*
 * INCLUDE FILES
 *****************************************************************************************
 */


#include "uart_dev_cfg.h"
#include "uart_dev.h"



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


/*
 * LOCAL FUNCTION DEFINITIONS
 *****************************************************************************************
 */
static void _uart_dev_init_default(void *const me)
{

}
static  void _uart_dev_deinit_default(void *const me)
{

}

static  int _uart_dev_send_default(void *const me, uint8_t *data, uint16_t len)
{
  return 0;
}
static  int _uart_dev_recv_default(void *const me, uint8_t *data, uint16_t len)
{
  return 0;
}
/*
 * GLOBAL FUNCTION DEFINITIONS
 *****************************************************************************************
 */


void uart_dev_ctor(uart_dev_t *const me)
{
  static const uart_dev_inf_t default_inf =
  {
    _uart_dev_init_default,
    _uart_dev_deinit_default,
    _uart_dev_send_default,
    _uart_dev_recv_default
  };
  me->inf = &default_inf;
  me->p_arg = me;
}



void uart_dev_init(uart_dev_t *const me)
{
  if(me && me->inf && me->inf->init)
  {
    me->inf->init(me->p_arg);
  }
}

void uart_dev_deinit(uart_dev_t *const me)
{
  if(me && me->inf && me->inf->deinit)
  {
    me->inf->deinit(me->p_arg);
  }
}

int uart_dev_send_data(uart_dev_t *const me, uint8_t *data, uint16_t len)
{
  int ret = 0;
  if(me && me->inf && me->inf->send_data)
  {
    ret = me->inf->send_data(me->p_arg, data, len);
  }
  return ret;
}


int uart_dev_recv_data(uart_dev_t *const me, uint8_t *data, uint16_t len)
{
  int ret = 0;
  if(me && me->inf && me->inf->recv_data)
  {
     ret = me->inf->recv_data(me->p_arg, data, len);
  }
  return ret;
}


/********************************************* END OF FILE *********************************************/
