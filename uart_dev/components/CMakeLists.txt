



set(src "uart_dev/uart.c" "uart_dev/uart_dev.c" "uart_dev/atk_denesp32s3_uart_dev/atk_denesp32s3_uart_dev.c")
set(inc "uart_dev/" "uart_dev/atk_denesp32s3_uart_dev")


idf_component_register(SRCS "${src}"
                    INCLUDE_DIRS "${inc}"
                    REQUIRES driver
                    )
