#include <stdio.h>

#include "esp_system.h"
#include "esp_wifi.h"
#include "esp_event.h"
#include "esp_log.h"
#include "nvs_flash.h"

static const char *TAG = "wifi scan";
void app_main(void)
{
    esp_err_t ret = nvs_flash_init();
    if ((ret == ESP_ERR_NVS_NO_FREE_PAGES) || (ret == ESP_ERR_NVS_NEW_VERSION_FOUND))
    {
        ESP_ERROR_CHECK(nvs_flash_erase());
        ret = nvs_flash_init();
    }
    ESP_ERROR_CHECK(ret);
    /*
     * 以下步骤需要严格参数  https://docs.espressif.com/projects/esp-idf/zh_CN/v5.4/esp32s3/api-guides/wifi.html 如下段落:
     * 1. ESP32-S3 Wi-Fi station 一般情况
     * 2. 在所有信道中扫描全部 AP（前端）
     */
    ESP_LOGI(TAG, "第一阶段:初始化阶段");
    /*s1.1：主任务通过调用函数 esp_netif_init() 创建一个 LwIP 核心任务，并初始化 LwIP 相关工作*/
    ESP_ERROR_CHECK(esp_netif_init());
    /**
     * s1.2：主任务通过调用函数 esp_event_loop_create() 创建一个系统事件任务，并初始化应用程序事件的回调函数。在此情况下，该回调函数唯一的动作就是将事件中继到应用程序任务中。
     * 这里注意:默认事件循环应该使用 esp_err_t esp_event_loop_create_default(void)
     * 参考 https://docs.espressif.com/projects/esp-idf/zh_CN/v5.4/esp32s3/api-reference/system/esp_event.html
     * */
    ESP_ERROR_CHECK(esp_event_loop_create_default());
    /**
     * s1.3：主任务通过调用函数 esp_netif_create_default_wifi_ap() 或 esp_netif_create_default_wifi_sta() 创建有 TCP/IP 堆栈的默认网络接口实例绑定 station 或 AP。
     * 注意:
     * 1. WIFI SCAN 使用station模式,扫描周围的WIFI信号
     * 2. 此函数有返回值,暂且忽略
     */
    esp_netif_create_default_wifi_sta();

    /**
     * s1.4：主任务通过调用函数 esp_wifi_init() 创建 Wi-Fi 驱动程序任务，并初始化 Wi-Fi 驱动程序
     * s1.3和s1.4对应 <ESP32-S3 Wi-Fi station 一般情况>图中的1.3
     */
    const wifi_init_config_t wifi_init_cfg = WIFI_INIT_CONFIG_DEFAULT();
    ESP_ERROR_CHECK(esp_wifi_init(&wifi_init_cfg));

    /*
     *s1.5：主任务通过调用 OS API 创建应用程序任务
     * 暂时省略
     *
     */

    ESP_LOGI(TAG, "第二阶段:配置阶段");
    ESP_ERROR_CHECK(esp_wifi_set_mode(WIFI_MODE_STA));

    ESP_LOGI(TAG, "第三阶段:启动阶段");
    /**
     * s3.1：调用函数 esp_wifi_start() 启动 Wi-Fi 驱动程序。
     * s3.2：Wi-Fi 驱动程序将事件 WIFI_EVENT_STA_START 发布到事件任务中，然后，事件任务将执行一些正常操作并调用应用程序的事件回调函数。
     *
     */
    ESP_ERROR_CHECK(esp_wifi_start());

    ESP_LOGI(TAG, "第四阶段:扫描配置阶段");
    /**
     * 参考如下章节:
     * 在所有信道中扫描全部 AP（前端）
     */
    /**
     * s1.1：如果默认的国家信息有误，调用函数 esp_wifi_set_country() 进行配置。请参阅 Wi-Fi 国家/地区代码。
     * 
     */
    wifi_country_t wifi_coutry_cfg = {
        .cc = "CN",
        .schan = 1,
        .nchan = 13,
    };
    ESP_ERROR_CHECK(esp_wifi_set_country(&wifi_coutry_cfg));

    /**
     * s1.2：调用函数 esp_wifi_scan_start() 配置扫描信息，可参阅 扫描配置。该场景为全信道扫描，将 SSID/BSSID/channel 设置为 0 即可。
     * 字段     描述
     * 
     * ssid    如果该字段的值不为 NULL，则仅可扫描到具有相同 SSID 值的 AP。
     * bssid   如果该字段的值不为 NULL，则仅可扫描到具有相同 BSSID 值的 AP。
     * channel 如果该字段值为 0，将进行全信道扫描；反之，将针对特定信道进行扫描。
     * show_hidden 如果该字段值为 0，本次扫描将忽略具有隐藏 SSID 的 AP；反之，这些 AP 也会在扫描时被视为正常 AP。
     * scan_type 如果该字段值为为 WIFI_SCAN_TYPE_ACTIVE，则本次扫描为主动扫描；反之，将被视为被动扫描。
     * scan_time 该字段用于控制每个信道的扫描时间。
     * 被动扫描时，scan_time.passive 字段负责为每个信道指定扫描时间。
     * 主动扫描时，每个信道的扫描时间如下列表所示。其中，min 代表 scan_time_active_min，max 代表 scan_time_active_max。
     * 
        min=0, max=0：每个信道的扫描时间为 120 ms。
        min>0, max=0：每个信道的扫描时间为 120 ms。
        min=0, max>0：每个信道的扫描时间为 max ms。
        min>0, max>0：每个信道扫描的最短时间为 min ms。 如果在这段时间内未找到 AP，将跳转至下一个信道。如这段时间内找到 AP，则该信道的扫描时间为 max ms。
        如希望提升 Wi-Fi 扫描性能，则可修改上述两个参数。
     *
     */
    // const wifi_scan_config_t wifi_scan_cfg = {
    //     .ssid = NULL,
    //     .bssid = NULL,
    //     .scan_type = 
    // };
    /* 设置为NULL 使用默认参数，可以查看此函数的说明,true 表示是堵塞,每个信道是扫描120ms
     * 根据coutry设置 扫描13个信号  120*13 = 1560ms
     */


    ESP_ERROR_CHECK(esp_wifi_scan_start(NULL, true));

    /**
     * 
     * s2.1：Wi-Fi 驱动程序切换至信道 1，此时的扫描类型为 WIFI_SCAN_TYPE_ACTIVE，同时发送一个 probe request。反之，Wi-Fi 将等待接收 AP beacon。Wi-Fi 驱动程序将在信道 1 停留一段时间。min/max 扫描时间中定义了 Wi-Fi 在信道 1 中停留的时间长短，默认为 120 ms。
     * s2.2：Wi-Fi 驱动程序跳转至信道 2，并重复进行 s2.1 中的步骤。
     * s2.3：Wi-Fi 驱动程序扫描最后的信道 N，N 的具体数值由步骤 s1.1 中配置的国家代码决定。
     * 
     * 
     */

    /****
     * 
     * s3.1：当所有信道扫描全部完成后，将产生 WIFI_EVENT_SCAN_DONE 事件。
     * s3.2：应用程序的事件回调函数告知应用程序任务已接收到 WIFI_EVENT_SCAN_DONE 事件。
     * 调用函数 esp_wifi_scan_get_ap_num() 获取在本次扫描中找到的 AP 数量。
     * 然后，分配出足够的事物槽，并调用函数 esp_wifi_scan_get_ap_records() 获取 AP 记录。
     * 请注意，一旦调用 esp_wifi_scan_get_ap_records()，Wi-Fi 驱动程序中的 AP 记录将被释放。
     * 但是，请不要在单个扫描完成事件中重复调用两次 esp_wifi_scan_get_ap_records()。
     * 反之，如果扫描完成事件发生后未调用 esp_wifi_scan_get_ap_records()，则 Wi-Fi 驱动程序中的 AP 记录不会被释放。因此，请务必确保调用函数 esp_wifi_scan_get_ap_records()，且仅调用一次。
     * 
     * 
     */

#define WIFI_SCAN_MAX_AP_RECORED_NUM   50u
    uint16_t wifi_scan_ap_num = 0;
    wifi_ap_record_t wifi_scan_ap_recored[WIFI_SCAN_MAX_AP_RECORED_NUM] = {0};

    ESP_ERROR_CHECK(esp_wifi_scan_get_ap_num(&wifi_scan_ap_num));
    ESP_LOGI(TAG, "wifi scan ap count: %d", wifi_scan_ap_num);


    ESP_ERROR_CHECK(esp_wifi_scan_get_ap_records(&wifi_scan_ap_num,wifi_scan_ap_recored));
    printf("%30s %s %s %s\n", "SSID", "频道", "强度", "MAC地址");
    for (uint16_t i = 0; i < wifi_scan_ap_num;i++)
    {
        printf("%30s  %3d  %3d  %02X-%02X-%02X-%02X-%02X-%02X\n", wifi_scan_ap_recored[i].ssid, wifi_scan_ap_recored[i].primary, wifi_scan_ap_recored[i].rssi, wifi_scan_ap_recored[i].bssid[0], wifi_scan_ap_recored[i].bssid[1], wifi_scan_ap_recored[i].bssid[2], wifi_scan_ap_recored[i].bssid[3], wifi_scan_ap_recored[i].bssid[4], wifi_scan_ap_recored[i].bssid[5]);
    }

    vTaskDelete(NULL);
}
