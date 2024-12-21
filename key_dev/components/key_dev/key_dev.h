/**
****************************************************************************************
* @file       key_dev.h
* @author     
* @version    V01
* @date       2024.12.21
* @brief       
  ****************************************************************************************
  */

#ifndef __KEY_DEV_H__
#define __KEY_DEV_H__


/* Includes ------------------------------------------------------------------*/
#include <stdint.h>


#ifdef __cplusplus
extern "C"
{
#endif
/* Exported types ------------------------------------------------------------*/
typedef struct _key_dev_inf_t
{
    void (*key_dev_init)(void *const me);
    uint8_t (*key_dev_read_state)(void *const me, const  uint32_t key_id);
} key_dev_inf_t;

typedef struct _key_dev_t
{
    const key_dev_inf_t *p_inf;
    void *p_param;

    uint32_t key_id;

    struct _key_dev_t *p_next;
} key_dev_t;

/* Exported functions --------------------------------------------------------*/

void key_dev_all_init(void);
void key_dev_init(key_dev_t *const me);
void key_dev_ctor(key_dev_t *const me);
void key_dev_register(key_dev_t *const me);
uint8_t key_dev_read_state(key_dev_t *const me,const uint32_t key_id);
uint8_t key_dev_read_state_depend_id(const uint32_t id);

#ifdef __cplusplus
}
#endif





#endif
/********************* (C) COPYRIGHT LonBon HardWare Team *******END OF FILE ********/
