
set(srcs    "led_dev/led_dev.c" 
            "led_dev/virtual_led.c"
            "led_dev/atk_denesp32s3_led.c")

idf_component_register(SRCS ${srcs}
                    INCLUDE_DIRS "led_dev"
                    REQUIRES driver)