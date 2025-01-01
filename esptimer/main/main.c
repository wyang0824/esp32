#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_timer.h"
#define LOG_LOCAL_LEVEL ESP_LOG_VERBOSE
#include "esp_log.h"



static const char* TAG = "APP_MAIN";
static esp_timer_handle_t s_ptesp_timer;

static void esp_timer_cb(void *arg)
{
    ESP_LOGI(TAG,"esp_timer_cb\n");
}

static void app_timer_init(void)
{
    esp_timer_create_args_t timer_conf = {
        .callback = esp_timer_cb,
        .arg = NULL,
        .dispatch_method = ESP_TIMER_TASK,
        .name = "timer"
    };
    ESP_ERROR_CHECK(esp_timer_create(&timer_conf, &s_ptesp_timer));
    ESP_ERROR_CHECK(esp_timer_start_periodic(s_ptesp_timer, 1000000));
}


void app_main(void)
{   
    app_timer_init();
    while(1) {
        vTaskDelay(1000 / portTICK_PERIOD_MS);
    }   
}
