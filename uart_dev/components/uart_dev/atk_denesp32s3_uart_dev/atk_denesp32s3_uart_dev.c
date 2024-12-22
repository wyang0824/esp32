/**
****************************************************************************************
* @file       atk_denesp32s3_uart_dev.c
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


#include "atk_denesp32s3_uart_dev_cfg.h"
#include "atk_denesp32s3_uart_dev.h"

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/uart.h"
#include "driver/gpio.h"
#include "sdkconfig.h"
#include "esp_log.h"
#include "esp_err.h"
/*
 * DEFINES
 *****************************************************************************************
 */

#define UART_RX_BUF_SIZE (1024)
#define UART_TX_BUF_SIZE (0)

#define UART_PORT UART_NUM_0


#define UART_TX_PIN (GPIO_NUM_43)
#define UART_RX_PIN (GPIO_NUM_44)
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
static void _uart_init(void *const me)
{
    atk_denesp32s3_uart_dev_t *const me_ = (atk_denesp32s3_uart_dev_t *)me;
    /* Configure parameters of an UART driver,
     * communication pins and install the driver */
    uart_config_t uart_config = {
      .baud_rate = me_->cfg.baud_rate,
      .data_bits = me_->cfg.data_bits,
      .parity = me_->cfg.parity,
      .stop_bits = me_->cfg.stop_bits,
      .flow_ctrl = me_->cfg.flow_ctrl,
      .source_clk = UART_SCLK_APB,// UART_SCLK_DEFAULT,
    };
    int intr_alloc_flags = 0;

    ESP_ERROR_CHECK(uart_driver_install(UART_PORT, UART_RX_BUF_SIZE * 2, 0, 0, NULL, intr_alloc_flags));
    ESP_ERROR_CHECK(uart_param_config(UART_PORT, &uart_config));
    ESP_ERROR_CHECK(uart_set_pin(UART_PORT, UART_TX_PIN, UART_RX_PIN, UART_PIN_NO_CHANGE, UART_PIN_NO_CHANGE));

}
static void _uart_deinit(void *const me)
{
  ESP_ERROR_CHECK(uart_driver_delete(UART_PORT));
}

static int _uart_send_data(void *const me, uint8_t *data, uint16_t len)
{
  return  uart_write_bytes(UART_PORT, (const char *) data, len); //这个是阻塞的发送 因为uart_driver_install tx_buf_size 设置为0
}
static int _uart_recv_data(void *const me, uint8_t *data, uint16_t len)
{
    return  uart_read_bytes(UART_PORT, data, len, 20 / portTICK_PERIOD_MS);

}

/*
 * GLOBAL FUNCTION DEFINITIONS
 *****************************************************************************************
 */

void atk_denesp32s3_uart_ctor(atk_denesp32s3_uart_dev_t *const me, atk_denesp32s3_uart_cfg_t *const cfg)
{
  static const uart_dev_inf_t default_inf =
  {
    _uart_init,
    _uart_deinit,
    _uart_send_data,
    _uart_recv_data
  };
  uart_dev_ctor(&me->super);

  me->super.inf = &default_inf;
  me->super.p_arg = me;

  me->cfg = *cfg; /*直接赋值*/
}








/********************************************* END OF FILE *********************************************/
