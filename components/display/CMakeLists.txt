idf_component_register(SRCS "src/display.c"
                    INCLUDE_DIRS "include"
                    REQUIRES common bus_manager
                    PRIV_REQUIRES driver)
