#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <inttypes.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/queue.h"
#include "driver/gpio.h"
#include "nvs_flash.h"


static void _led_init(void)
{
    gpio_config_t io_conf = {};
    io_conf.intr_type = GPIO_INTR_DISABLE;
    io_conf.pin_bit_mask = 1 << GPIO_NUM_1;
    io_conf.mode = GPIO_MODE_INPUT_OUTPUT; // GPIO_MODE_OUTPUT; 如果没有设置输入模式无法使用gpio_get_level 可以查看 gpio_hal_get_level接口描述
    io_conf.pull_up_en = GPIO_PULLUP_ENABLE;
    io_conf.pull_down_en = GPIO_PULLDOWN_DISABLE;
    ESP_ERROR_CHECK(gpio_config(&io_conf));
}
static void _led_toggle(void)
{
    gpio_set_level(GPIO_NUM_1, !gpio_get_level(GPIO_NUM_1)); /*设置为只输出模式,无法通过gpio_get_level获取电平*/
}
static void _led_set_level(const uint32_t level)
{
    gpio_set_level(GPIO_NUM_1, level);
}
void app_main(void)
{
    #if 0
    esp_err_t ret;
    ret = nvs_flash_init();
    if((ret == ESP_ERR_NVS_NO_FREE_PAGES || ret == ESP_ERR_NVS_NEW_VERSION_FOUND))
    {
        ESP_ERROR_CHECK(nvs_flash_erase());
        ret = nvs_flash_init();
    }
    #endif
    _led_init();
    while (1)
    {
        _led_set_level(1);
        printf("get gpio level:%d-%d\r\n", gpio_get_level(GPIO_NUM_1),!gpio_get_level(GPIO_NUM_1));
        vTaskDelay(1000);

        _led_set_level(0);
        printf("get gpio level:%d-%d\r\n", gpio_get_level(GPIO_NUM_1),!gpio_get_level(GPIO_NUM_1));

        vTaskDelay(1000);
    }
    
}
