
set(srcs "at24cxx-main/src/driver_at24cxx.c"
         "at24cxx-main/interface/driver_at24cxx_interface_template.c"
         "at24cxx-main/example/driver_at24cxx_basic.c")

set(inc_dirs "at24cxx-main/src"
             "at24cxx-main/interface"
             "at24cxx-main/example")

idf_component_register(SRCS ${srcs}
                    INCLUDE_DIRS ${inc_dirs}
                    REQUIRES driver)
