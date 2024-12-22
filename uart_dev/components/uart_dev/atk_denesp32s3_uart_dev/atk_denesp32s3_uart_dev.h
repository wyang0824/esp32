/**
****************************************************************************************
* @file       atk_denesp32s3_uart_dev.h
* @author     
* @version    V01
* @date       2024.12.22
* @brief       
  ****************************************************************************************
  */

#ifndef __ATK_DENESP32S3_UART_DEV_H__
#define __ATK_DENESP32S3_UART_DEV_H__


/* Includes ------------------------------------------------------------------*/
#include <stdint.h>
#include "uart_dev.h"

#ifdef __cplusplus
extern "C"
{
#endif
/* Exported types ------------------------------------------------------------*/
/*  配置要注意
 * 1. 可以添加引脚的的配置，这里先不做添加
 * 2. 需要定义应用的配置值，转换成每个芯片的配置值，这里直接使用芯片的配置值(需要查看芯片的配置值)
 */
typedef struct _atk_denesp32s3_uart_cfg_t 
{
  uint32_t baud_rate;
  uint8_t data_bits;
  uint8_t stop_bits;
  uint8_t parity;
  uint8_t flow_ctrl;
} atk_denesp32s3_uart_cfg_t;

typedef struct _atk_denesp32s3_uart_dev_t
{
  uart_dev_t super;
  atk_denesp32s3_uart_cfg_t cfg;
}atk_denesp32s3_uart_dev_t;

#if 0
typedef struct _atk_denesp32s3_uart_dev_inf_t
{
  void (*init)(void *const me);
  void (*deinit)(void *const me);

  int (*send_data)(void *const me, uint8_t *data, uint16_t len);
  int (*recv_data)(void *const me, uint8_t *data, uint16_t len);
} atk_denesp32s3_uart_dev_inf_t;
#endif


/* Exported functions --------------------------------------------------------*/

void atk_denesp32s3_uart_ctor(atk_denesp32s3_uart_dev_t *const me, atk_denesp32s3_uart_cfg_t *const cfg);

#ifdef __cplusplus
}
#endif





#endif
/********************************************* END OF FILE *********************************************/
