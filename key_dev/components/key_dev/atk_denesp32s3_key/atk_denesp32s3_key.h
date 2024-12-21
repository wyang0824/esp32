/**
****************************************************************************************
* @file       atk_denesp32s3_key.h
* @author     
* @version    V01
* @date       2024.12.21
* @brief       
  ****************************************************************************************
  */

#ifndef __ATK_DENESP32S3_KEY_H__
#define __ATK_DENESP32S3_KEY_H__


/* Includes ------------------------------------------------------------------*/
#include <stdint.h>

#include "key_dev.h"

#ifdef __cplusplus
extern "C"
{
#endif
/* Exported types ------------------------------------------------------------*/
#define ATK_DENESP32S3_KEY_CNT 1



typedef struct _atk_denesp32s3_key_inf_t
{
    void (*atk_denesp32s3_key_init)(void *const me);
    uint8_t (*atk_denesp32s3_key_read_state)(void *const me);
} atk_denesp32s3_key_inf_t;


typedef struct _atk_denesp32s3_key_t
{
    key_dev_t atk_denesp32s3_key_dev;  //多个io可以创建多个设备
    uint32_t  atk_denesp32s3_key_num;  //io 引脚编号
    //atk_denesp32s3_key_inf_t *p_inf;   //继续抽象接口
} atk_denesp32s3_key_t;

/* Exported functions --------------------------------------------------------*/

void atk_denesp32s3_key_ctor(atk_denesp32s3_key_t *const me, const uint32_t key_id, const uint32_t key_num);

#ifdef __cplusplus
}
#endif





#endif
/********************************************* END OF FILE *********************************************/
