#include <stdio.h>
#include <string.h>

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver_at24cxx_basic.h"

static uint8_t write_buf[256] = {0};
static uint8_t read_buf[256] = {0};

static at24cxx_handle_t gs_handle;        /**< at24cxx handle */
void app_main(void)
{
    do{
        volatile  uint16_t i = 0;
        uint8_t ret = at24cxx_basic_init(AT24C02, AT24CXX_ADDRESS_A000);

        if(ret)
        {
            printf("at24cxx_basic_init failed\n");
            break;
        }

        for(i = 0; i < sizeof(write_buf); i++)
        {
            write_buf[i] = i;
        }

        ret = at24cxx_basic_write(0, write_buf, sizeof(write_buf));
        if(ret)
        {
            printf("at24cxx_basic_init failed\n");
            break;
        }
        memset(read_buf, 0, sizeof(read_buf));

        ret = at24cxx_basic_read(0, read_buf, sizeof(read_buf));
        if (ret  == 0)
        {
            printf("read_buf: \n");
            for(i = 0; i < sizeof(read_buf); i++)
            {
                printf("%03d ", read_buf[i]);
                if( (i+1) % 8 == 0)
                {
                    printf("\n");
                }
            }
            printf("\n");
            }
    
    }while(0);

    while(1)
    {
        printf("at24cxx: run \n");
        vTaskDelay(1000 / portTICK_PERIOD_MS);
    }
}
