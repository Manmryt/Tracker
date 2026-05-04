#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_log.h"
#include "esp_timer.h"

#include "bus_manager.h"
#include "icm20948.h"
#include "ahrs.h"
#include "telemetry.h"
#include "logger.h"
#include "tracker_types.h"

static const char *TAG = "MAIN";

void app_main(void)
{
    tracker_state_t tracker = {0};
    int64_t last_time = esp_timer_get_time();
    
    // 1. Initialize Communication Buses (I2C/SPI)
    ESP_ERROR_CHECK(bus_manager_init_all());

    // 2. Initialize Hardware Components
    icm20948_handle_t imu = icm20948_create(I2C_MASTER_NUM, ICM20948_I2C_ADDR);
    ESP_ERROR_CHECK(icm20948_init(imu));
    
    // 3. Initialize Logger (SD Card)
    logger_init();

    // 4. Initialize Processing & Comm Layers
    ahrs_init(2.0f);
    telemetry_init();

    ESP_LOGI(TAG, "Tracker System Ready");

    while (1) {
        int64_t current_time = esp_timer_get_time();
        float dt = (float)(current_time - last_time) / 1000000.0f;
        last_time = current_time;

        // 5. Acquisition
        icm20948_read_accel(imu, &tracker.accel);
        icm20948_read_gyro(imu, &tracker.gyro);

        // 6. Processing
        ahrs_update(tracker.accel, tracker.gyro, dt, &tracker.attitude);

        // 7. Communication
        telemetry_send_state(tracker);

        // 8. Data Logging (Blackbox)
        logger_log_data(tracker);

        ESP_LOGI(TAG, "R:%5.1f | P:%5.1f | Y:%5.1f", 
                 tracker.attitude.roll, tracker.attitude.pitch, tracker.attitude.yaw);

        vTaskDelay(pdMS_TO_TICKS(10)); 
    }
}
