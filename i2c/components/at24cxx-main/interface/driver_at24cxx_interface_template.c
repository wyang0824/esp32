/**
 * Copyright (c) 2015 - present LibDriver All rights reserved
 * 
 * The MIT License (MIT)
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE. 
 *
 * @file      driver_at24cxx_interface_template.c
 * @brief     driver at24cxx interface template source file
 * @version   2.0.0
 * @author    Shifeng Li
 * @date      2021-02-17
 *
 * <h3>history</h3>
 * <table>
 * <tr><th>Date        <th>Version  <th>Author      <th>Description
 * <tr><td>2021/02/17  <td>2.0      <td>Shifeng Li  <td>format the code
 * <tr><td>2020/10/15  <td>1.0      <td>Shifeng Li  <td>first upload
 * </table>
 */

#include "driver_at24cxx_interface.h"
#include "esp_log.h"
#include "driver/i2c.h"
#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

static const char *TAG = "at24cxx";

#define I2C_MASTER_SCL_IO           42      /*!< GPIO number used for I2C master clock */
#define I2C_MASTER_SDA_IO           41      /*!< GPIO number used for I2C master data  */
#define I2C_MASTER_NUM              0                          /*!< I2C master i2c port number, the number of i2c peripheral interfaces available will depend on the chip */
#define I2C_MASTER_FREQ_HZ          400000                     /*!< I2C master clock frequency */
#define I2C_MASTER_TX_BUF_DISABLE   0                          /*!< I2C master doesn't need buffer */
#define I2C_MASTER_RX_BUF_DISABLE   0                          /*!< I2C master doesn't need buffer */
#define I2C_MASTER_TIMEOUT_MS       1000


/**
 * @brief  interface iic bus init
 * @return status code
 *         - 0 success
 *         - 1 iic init failed
 * @note   none
 */
uint8_t at24cxx_interface_iic_init(void)
{
    int i2c_master_port = I2C_MASTER_NUM;

    i2c_config_t conf = {
        .mode = I2C_MODE_MASTER,
        .sda_io_num = I2C_MASTER_SDA_IO,
        .scl_io_num = I2C_MASTER_SCL_IO,
        .sda_pullup_en = GPIO_PULLUP_DISABLE,
        .scl_pullup_en = GPIO_PULLUP_DISABLE,
        .master.clk_speed = I2C_MASTER_FREQ_HZ,
    };

    i2c_param_config(i2c_master_port, &conf);

    esp_err_t err = i2c_driver_install(i2c_master_port, conf.mode,
                                       I2C_MASTER_RX_BUF_DISABLE,
                                       I2C_MASTER_TX_BUF_DISABLE, 0);

                                       
    return (err == ESP_OK) ? 0 : 1;
}

/**
 * @brief  interface iic bus deinit
 * @return status code
 *         - 0 success
 *         - 1 iic deinit failed
 * @note   none
 */
uint8_t at24cxx_interface_iic_deinit(void)
{
    esp_err_t err = i2c_driver_delete(I2C_MASTER_NUM);
    return (err == ESP_OK) ? 0 : 1;
}

/**
 * @brief      interface iic bus read
 * @param[in]  addr iic device write address
 * @param[in]  reg iic register address
 * @param[out] *buf pointer to a data buffer
 * @param[in]  len length of the data buffer
 * @return     status code
 *             - 0 success
 *             - 1 read failed
 * @note       none
 */
uint8_t at24cxx_interface_iic_read(uint8_t addr, uint8_t reg, uint8_t *buf, uint16_t len)
{
    i2c_cmd_handle_t i2c_cmd_handle = i2c_cmd_link_create();
    i2c_master_start(i2c_cmd_handle);
    i2c_master_write_byte(i2c_cmd_handle, addr, true);
    i2c_master_write_byte(i2c_cmd_handle, reg, true);
    i2c_master_start(i2c_cmd_handle);
    i2c_master_write_byte(i2c_cmd_handle, addr+1, true);
    #if 0
    for(uint16_t i = 0; i < len; i++)
    {
        i2c_master_read_byte(i2c_cmd_handle, &buf[i], i == len - 1 ? I2C_MASTER_LAST_NACK : I2C_MASTER_ACK);
    }
    #else
    i2c_master_read(i2c_cmd_handle, buf, len-1, I2C_MASTER_ACK);
    i2c_master_read_byte(i2c_cmd_handle, &buf[len-1], I2C_MASTER_LAST_NACK);
    #endif
    i2c_master_stop(i2c_cmd_handle);

    esp_err_t ret = i2c_master_cmd_begin(I2C_MASTER_NUM, i2c_cmd_handle, I2C_MASTER_TIMEOUT_MS / portTICK_PERIOD_MS);
    i2c_cmd_link_delete(i2c_cmd_handle);

    return ret == ESP_OK ? 0 : 1;
}

/**
 * @brief     interface iic bus write
 * @param[in] addr iic device write address
 * @param[in] reg iic register address
 * @param[in] *buf pointer to a data buffer
 * @param[in] len length of the data buffer
 * @return    status code
 *            - 0 success
 *            - 1 write failed
 * @note      none
 */
uint8_t at24cxx_interface_iic_write(uint8_t addr, uint8_t reg, uint8_t *buf, uint16_t len)
{
    i2c_cmd_handle_t i2c_cmd_handle = i2c_cmd_link_create();
    i2c_master_start(i2c_cmd_handle);
    i2c_master_write_byte(i2c_cmd_handle, addr, true);
    i2c_master_write_byte(i2c_cmd_handle, reg, true);
    for(uint16_t i = 0; i < len; i++)
    {
        i2c_master_write_byte(i2c_cmd_handle, buf[i], true);
    }
    i2c_master_stop(i2c_cmd_handle);

    esp_err_t ret = i2c_master_cmd_begin(I2C_MASTER_NUM, i2c_cmd_handle, I2C_MASTER_TIMEOUT_MS / portTICK_PERIOD_MS);
    i2c_cmd_link_delete(i2c_cmd_handle);

    return ret == ESP_OK ? 0 : 1;
}

/**
 * @brief      interface iic bus read with 16 bits register address
 * @param[in]  addr iic device write address
 * @param[in]  reg iic register address
 * @param[out] *buf pointer to a data buffer
 * @param[in]  len length of the data buffer
 * @return     status code
 *             - 0 success
 *             - 1 read failed
 * @note       none
 */
uint8_t at24cxx_interface_iic_read_address16(uint8_t addr, uint16_t reg, uint8_t *buf, uint16_t len)
{
    i2c_cmd_handle_t i2c_cmd_handle = i2c_cmd_link_create();
    i2c_master_start(i2c_cmd_handle);
    i2c_master_write_byte(i2c_cmd_handle, addr, true);
    i2c_master_write_byte(i2c_cmd_handle, (uint8_t)((reg & 0xFF00) >> 8), true);
    i2c_master_write_byte(i2c_cmd_handle, (uint8_t)(reg & 0x00FF), true);


    i2c_master_start(i2c_cmd_handle);
    i2c_master_write_byte(i2c_cmd_handle, addr+1, true);
    #if 0
    for(uint16_t i = 0; i < len; i++)
    {
        i2c_master_read_byte(i2c_cmd_handle, &buf[i], i == len - 1 ? I2C_MASTER_LAST_NACK : I2C_MASTER_ACK);
    }
    #else
    i2c_master_read(i2c_cmd_handle, buf, len-1, I2C_MASTER_ACK);
    i2c_master_read_byte(i2c_cmd_handle, &buf[len-1], I2C_MASTER_LAST_NACK);
    #endif
    i2c_master_stop(i2c_cmd_handle);

    esp_err_t ret = i2c_master_cmd_begin(I2C_MASTER_NUM, i2c_cmd_handle, I2C_MASTER_TIMEOUT_MS / portTICK_PERIOD_MS);
    i2c_cmd_link_delete(i2c_cmd_handle);

    return ret == ESP_OK ? 0 : 1;
}

/**
 * @brief     interface iic bus write with 16 bits register address
 * @param[in] addr iic device write address
 * @param[in] reg iic register address
 * @param[in] *buf pointer to a data buffer
 * @param[in] len length of the data buffer
 * @return    status code
 *            - 0 success
 *            - 1 write failed
 * @note      none
 */
uint8_t at24cxx_interface_iic_write_address16(uint8_t addr, uint16_t reg, uint8_t *buf, uint16_t len)
{
    i2c_cmd_handle_t i2c_cmd_handle = i2c_cmd_link_create();
    i2c_master_start(i2c_cmd_handle);
    i2c_master_write_byte(i2c_cmd_handle, addr, true);
    i2c_master_write_byte(i2c_cmd_handle, (uint8_t)((reg & 0xFF00) >> 8), true);
    i2c_master_write_byte(i2c_cmd_handle, (uint8_t)(reg & 0x00FF), true);
    for(uint16_t i = 0; i < len; i++)
    {
        i2c_master_write_byte(i2c_cmd_handle, buf[i], true);
    }
    i2c_master_stop(i2c_cmd_handle);

    esp_err_t ret = i2c_master_cmd_begin(I2C_MASTER_NUM, i2c_cmd_handle, I2C_MASTER_TIMEOUT_MS / portTICK_PERIOD_MS);
    i2c_cmd_link_delete(i2c_cmd_handle);

    return ret == ESP_OK ? 0 : 1;
}

/**
 * @brief     interface delay ms
 * @param[in] ms time
 * @note      none
 */
void at24cxx_interface_delay_ms(uint32_t ms)
{
    vTaskDelay(ms / portTICK_PERIOD_MS);
}

/**
 * @brief     interface print format data
 * @param[in] fmt format data
 * @note      none
 */
void at24cxx_interface_debug_print(const char *const fmt, ...)
{
    uint8_t	buf[256] = {'\0'};
    va_list args;
    va_start(args, fmt);
    vsnprintf((char *)buf, 256, fmt, args);
    va_end(args);
    ESP_LOGI(TAG, "%s", buf);
}
