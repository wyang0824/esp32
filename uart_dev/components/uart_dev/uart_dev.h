/**
****************************************************************************************
* @file       uart_dev.h
* @author     
* @version    V01
* @date       2024.12.22
* @brief       
  ****************************************************************************************
  */

#ifndef __UART_DEV_H__
#define __UART_DEV_H__


/* Includes ------------------------------------------------------------------*/
#include <stdint.h>


#ifdef __cplusplus
extern "C"
{
#endif
/* Exported types ------------------------------------------------------------*/
typedef struct _uart_dev_inf_t
{
  void (*init)(void *const me);
  void (*deinit)(void *const me);

  int (*send_data)(void *const me, uint8_t *data, uint16_t len);
  int (*recv_data)(void *const me, uint8_t *data, uint16_t len);
} uart_dev_inf_t;




typedef struct _uart_dev_t
{
  const uart_dev_inf_t *inf;
  void *p_arg;
} uart_dev_t;
/* Exported functions --------------------------------------------------------*/

void uart_dev_ctor(uart_dev_t *const me);

void uart_dev_init(uart_dev_t *const me);
void uart_dev_deinit(uart_dev_t *const me);
int uart_dev_send_data(uart_dev_t *const me, uint8_t *data, uint16_t len);
int uart_dev_recv_data(uart_dev_t *const me, uint8_t *data, uint16_t len);

#ifdef __cplusplus
}
#endif





#endif
/********************************************* END OF FILE *********************************************/
