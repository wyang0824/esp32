#include <stdio.h>

#include "nvs_flash.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/event_groups.h"

#include "esp_log.h"
#include "esp_wifi.h"
#include "esp_event.h"
#include "esp_system.h"

/* The examples use WiFi configuration that you can set via project configuration menu

   If you'd rather not, just change the below entries to strings with
   the config you want - ie #define EXAMPLE_WIFI_SSID "mywifissid"
*/
#define EXAMPLE_ESP_WIFI_SSID      CONFIG_ESP_WIFI_SSID
#define EXAMPLE_ESP_WIFI_PASS      CONFIG_ESP_WIFI_PASSWORD
#define EXAMPLE_ESP_MAXIMUM_RETRY  CONFIG_ESP_MAXIMUM_RETRY

#if CONFIG_ESP_WPA3_SAE_PWE_HUNT_AND_PECK
#define ESP_WIFI_SAE_MODE WPA3_SAE_PWE_HUNT_AND_PECK
#define EXAMPLE_H2E_IDENTIFIER ""
#elif CONFIG_ESP_WPA3_SAE_PWE_HASH_TO_ELEMENT
#define ESP_WIFI_SAE_MODE WPA3_SAE_PWE_HASH_TO_ELEMENT
#define EXAMPLE_H2E_IDENTIFIER CONFIG_ESP_WIFI_PW_ID
#elif CONFIG_ESP_WPA3_SAE_PWE_BOTH
#define ESP_WIFI_SAE_MODE WPA3_SAE_PWE_BOTH
#define EXAMPLE_H2E_IDENTIFIER CONFIG_ESP_WIFI_PW_ID
#endif
#if CONFIG_ESP_WIFI_AUTH_OPEN
#define ESP_WIFI_SCAN_AUTH_MODE_THRESHOLD WIFI_AUTH_OPEN
#elif CONFIG_ESP_WIFI_AUTH_WEP
#define ESP_WIFI_SCAN_AUTH_MODE_THRESHOLD WIFI_AUTH_WEP
#elif CONFIG_ESP_WIFI_AUTH_WPA_PSK
#define ESP_WIFI_SCAN_AUTH_MODE_THRESHOLD WIFI_AUTH_WPA_PSK
#elif CONFIG_ESP_WIFI_AUTH_WPA2_PSK
#define ESP_WIFI_SCAN_AUTH_MODE_THRESHOLD WIFI_AUTH_WPA2_PSK
#elif CONFIG_ESP_WIFI_AUTH_WPA_WPA2_PSK
#define ESP_WIFI_SCAN_AUTH_MODE_THRESHOLD WIFI_AUTH_WPA_WPA2_PSK
#elif CONFIG_ESP_WIFI_AUTH_WPA3_PSK
#define ESP_WIFI_SCAN_AUTH_MODE_THRESHOLD WIFI_AUTH_WPA3_PSK
#elif CONFIG_ESP_WIFI_AUTH_WPA2_WPA3_PSK
#define ESP_WIFI_SCAN_AUTH_MODE_THRESHOLD WIFI_AUTH_WPA2_WPA3_PSK
#elif CONFIG_ESP_WIFI_AUTH_WAPI_PSK
#define ESP_WIFI_SCAN_AUTH_MODE_THRESHOLD WIFI_AUTH_WAPI_PSK
#endif


/* The event group allows multiple bits for each event, but we only care about two events:
 * - we are connected to the AP with an IP
 * - we failed to connect after the maximum amount of retries */
#define WIFI_START_BIT          BIT0
#define WIFI_SCAN_DONE_BIT      BIT1
#define WIFI_GET_IP_BIT         BIT2
#define WIFI_FAIL_BIT           BIT3
#define WIFI_CONNECTED_BIT      BIT4

static void _wifi_init(void);


static const char *TAG = "app main";
// ESP_EVENT_DEFINE_BASE(TASK_EVENTS);
static TaskHandle_t app_task_handle = NULL;
/* FreeRTOS event group to signal when we are connected*/
static EventGroupHandle_t s_wifi_event_group;

static uint8_t s_retry_num = 0;

void app_main(void)
{
    esp_err_t ret = nvs_flash_init();
    if ((ret == ESP_ERR_NVS_NO_FREE_PAGES) || (ret == ESP_ERR_NVS_NEW_VERSION_FOUND))
    {
        ESP_ERROR_CHECK(nvs_flash_erase());
        ret = nvs_flash_init();
    }
    ESP_ERROR_CHECK(ret);

    _wifi_init();

    vTaskDelete(NULL);
}

    


static void  wifi_scan_evt_loop_handle(void* event_handler_arg,esp_event_base_t event_base,int32_t event_id,void* event_data)
{
    ESP_LOGI(TAG, "EVENT_BASE:%s EVENT_ID: %ld", event_base,event_id); /*注意 event_id是int32_t 类型 必须使用%ld打印 使用%d报错 */
    if ((event_base == WIFI_EVENT) && (event_id == WIFI_EVENT_STA_START))
    {
        ESP_LOGI(TAG, "esp wifi start evt");
        esp_wifi_connect();
    }
    else if  ((event_base == WIFI_EVENT) && (event_id == WIFI_EVENT_SCAN_DONE))
    {
        ESP_LOGI(TAG, "esp wifi scan done evt");
        xEventGroupSetBits(s_wifi_event_group, WIFI_SCAN_DONE_BIT);
    }
    else if ((event_base == WIFI_EVENT) && (event_id == WIFI_EVENT_STA_DISCONNECTED))
    {
            if (s_retry_num < EXAMPLE_ESP_MAXIMUM_RETRY) {
            esp_wifi_connect();
            s_retry_num++;
            ESP_LOGI(TAG, "retry to connect to the AP");
            }
            else {
            xEventGroupSetBits(s_wifi_event_group, WIFI_FAIL_BIT);
            }
    }
    else if((event_base == IP_EVENT) && (event_id == IP_EVENT_STA_GOT_IP))
    {
        ip_event_got_ip_t* event = (ip_event_got_ip_t*) event_data;
        ESP_LOGI(TAG, "got ip:" IPSTR, IP2STR(&event->ip_info.ip));
        s_retry_num = 0;
        xEventGroupSetBits(s_wifi_event_group, WIFI_CONNECTED_BIT);
    }
    else
    {
        ; /*None*/
    }
}


static void app_task( void * pvParameters )
{
    esp_event_handler_instance_t instance_any_id;
    esp_event_handler_instance_t instance_got_ip;

    ESP_ERROR_CHECK(esp_event_handler_register(WIFI_EVENT, ESP_EVENT_ANY_ID, &wifi_scan_evt_loop_handle, &instance_any_id));
    ESP_ERROR_CHECK(esp_event_handler_register(IP_EVENT,   ESP_EVENT_ANY_ID, &wifi_scan_evt_loop_handle, &instance_got_ip));
    while(1)
    {
        EventBits_t bits = xEventGroupWaitBits(s_wifi_event_group,
                WIFI_START_BIT | WIFI_SCAN_DONE_BIT | WIFI_GET_IP_BIT | WIFI_FAIL_BIT | WIFI_CONNECTED_BIT,
                pdTRUE,
                pdFALSE,
                portMAX_DELAY);
        if(bits & WIFI_CONNECTED_BIT)
        {
            ESP_LOGI(TAG, "wifi connect");
        }
        else if(bits & WIFI_FAIL_BIT)
        {
            ESP_LOGI(TAG, "wifi disconnect");
        }
        else if(bits & WIFI_GET_IP_BIT)
        {
            ESP_LOGI(TAG, "get ip success");
        }
    }
}


static void _wifi_init(void)
{
    ESP_ERROR_CHECK(esp_netif_init());

    ESP_ERROR_CHECK(esp_event_loop_create_default());
    esp_netif_t *p_netif = esp_netif_create_default_wifi_sta();
    assert(p_netif);

    const wifi_init_config_t wifi_init_cfg = WIFI_INIT_CONFIG_DEFAULT();
    ESP_ERROR_CHECK(esp_wifi_init(&wifi_init_cfg));


    s_wifi_event_group = xEventGroupCreate();
    configASSERT(s_wifi_event_group);
    xTaskCreate( app_task, "app task", 40960, NULL, uxTaskPriorityGet(NULL) + 1, &app_task_handle );
    configASSERT( app_task_handle );   


    ESP_ERROR_CHECK(esp_wifi_set_mode(WIFI_MODE_STA));

    wifi_country_t wifi_coutry_cfg = {
        .cc = "CN",
        .schan = 1,
        .nchan = 13,
    };
    ESP_ERROR_CHECK(esp_wifi_set_country(&wifi_coutry_cfg));

    wifi_config_t wifi_cfg = {
        .sta = {
            .ssid = EXAMPLE_ESP_WIFI_SSID,
            .password = EXAMPLE_ESP_WIFI_PASS,
            .threshold.authmode = ESP_WIFI_SCAN_AUTH_MODE_THRESHOLD,
            .sae_pwe_h2e = ESP_WIFI_SAE_MODE,
            .sae_h2e_identifier = EXAMPLE_H2E_IDENTIFIER,
        },
    };
    ESP_ERROR_CHECK(esp_wifi_set_config(WIFI_IF_STA, &wifi_cfg));

    ESP_ERROR_CHECK(esp_wifi_start());


}