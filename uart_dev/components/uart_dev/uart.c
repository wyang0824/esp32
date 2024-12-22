/**
****************************************************************************************
* @file       uart.c
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


#include "uart_cfg.h"
#include "uart.h"


#include "atk_denesp32s3_uart_dev.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"


#include "esp_log.h"
#include "esp_err.h"

#include <stdio.h>

/*
 * DEFINES
 *****************************************************************************************
 */
 #define BUF_SIZE (1024)


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

 static const char *s_cstrtag = "UART";

 static atk_denesp32s3_uart_dev_t s_tatk_esp32s3_uart_dev;
/*
 * LOCAL FUNCTION DEFINITIONS
 *****************************************************************************************
 */

static void _uart_task(void *arg)
{
    atk_denesp32s3_uart_dev_t *p_uart_dev = (atk_denesp32s3_uart_dev_t *) arg;   
    // Configure a temporary buffer for the incoming data
    uint8_t *data = (uint8_t *) malloc(BUF_SIZE);

    while (1) {
        // Read data from the UART
        int len = uart_dev_recv_data(&p_uart_dev->super, data, BUF_SIZE);
        // Write data back to the UART
        uart_dev_send_data(&p_uart_dev->super, data, len);
        if (len) {
            data[len] = '\0';
            ESP_LOGI(s_cstrtag, "Recv str: %s", (char *) data);
        }
    }
}
/*
 * GLOBAL FUNCTION DEFINITIONS
 *****************************************************************************************
 */

void uart_ctor(void)
{
    ESP_LOGI(s_cstrtag, "uart_ctor");
    atk_denesp32s3_uart_cfg_t cfg = {
        .baud_rate = 115200,
        .data_bits = 0x03,
        .parity = 0x00,
        .stop_bits = 0x01,
        .flow_ctrl = 0x00,
    };
    atk_denesp32s3_uart_ctor(&s_tatk_esp32s3_uart_dev,&cfg);
    uart_dev_init(&s_tatk_esp32s3_uart_dev.super);
    xTaskCreate(_uart_task, "uart_task", 4096, &s_tatk_esp32s3_uart_dev, 10, NULL);
}










/********************************************* END OF FILE *********************************************/
