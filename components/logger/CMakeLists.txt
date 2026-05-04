idf_component_register(SRCS "src/logger.c"
                    INCLUDE_DIRS "include"
                    REQUIRES common bus_manager fatfs sdmmc)
