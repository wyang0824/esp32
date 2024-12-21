set(srcs  "key_dev/key.c" "key_dev/MultiButton/multi_button.c" "key_dev/key_dev.c" "key_dev/atk_denesp32s3_key/atk_denesp32s3_key.c") 
set(inc_dir "key_dev/" "key_dev/MultiButton" "key_dev/atk_denesp32s3_key")

idf_component_register(SRCS ${srcs}
                    INCLUDE_DIRS ${inc_dir}
                    REQUIRES driver)



message(STATUS "srcs = ${srcs}")
message(STATUS "inc_dir = ${inc_dir}")