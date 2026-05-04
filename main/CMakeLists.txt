idf_component_register(SRCS "main.c"
                    INCLUDE_DIRS "."
                    REQUIRES bus_manager icm20948 ahrs telemetry common logger)
