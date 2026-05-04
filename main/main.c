#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_log.h"
#include "esp_timer.h"
#include "nvs_flash.h"
#include "driver/gpio.h"

#include "bus_manager.h"
#include "icm20948.h"
#include "ahrs.h"
#include "telemetry.h"
#include "logger.h"
#include "tracker_types.h"

static const char *TAG = "MAIN";

#define CALIBRATION_BUTTON_IO    0 // Boot button on most devkits

static esp_err_t save_offsets(icm20948_offsets_t offsets)
{
    nvs_handle_t handle;
    esp_err_t err = nvs_open("storage", NVS_READWRITE, &handle);
    if (err != ESP_OK) return err;

    err = nvs_set_blob(handle, "offsets", &offsets, sizeof(offsets));
    if (err == ESP_OK) nvs_commit(handle);

    nvs_close(handle);
    return err;
}

static esp_err_t load_offsets(icm20948_offsets_t *offsets)
{
    nvs_handle_t handle;
    esp_err_t err = nvs_open("storage", NVS_READONLY, &handle);
    if (err != ESP_OK) return err;

    size_t size = sizeof(icm20948_offsets_t);
    err = nvs_get_blob(handle, "offsets", offsets, &size);

    nvs_close(handle);
    return err;
}

void app_main(void)
{
    tracker_state_t tracker = {0};
    icm20948_offsets_t offsets = {0};
    int64_t last_time = esp_timer_get_time();

    // 1. Initialize NVS
    esp_err_t ret = nvs_flash_init();
    if (ret == ESP_ERR_NVS_NO_FREE_PAGES || ret == ESP_ERR_NVS_NEW_VERSION_FOUND) {
        ESP_ERROR_CHECK(nvs_flash_erase());
        ret = nvs_flash_init();
    }
    ESP_ERROR_CHECK(ret);

    // 2. Initialize Communication Buses
    ESP_ERROR_CHECK(bus_manager_init_all());

    // 3. Initialize IMU
    icm20948_handle_t imu = icm20948_create(I2C_MASTER_NUM, ICM20948_I2C_ADDR);
    ESP_ERROR_CHECK(icm20948_init(imu));

    // 4. Calibration Logic
    gpio_set_direction(CALIBRATION_BUTTON_IO, GPIO_MODE_INPUT);
    if (gpio_get_level(CALIBRATION_BUTTON_IO) == 0) {
        ESP_LOGI(TAG, "Calibration requested via button...");
        icm20948_calibrate(imu, &offsets);
        save_offsets(offsets);
    } else {
        if (load_offsets(&offsets) == ESP_OK) {
            ESP_LOGI(TAG, "Loaded calibration offsets from NVS");
        } else {
            ESP_LOGW(TAG, "No calibration data found. Using defaults.");
        }
    }
    icm20948_apply_offsets(imu, offsets);

    // 5. Initialize Logger & Layers
    logger_init();
    ahrs_init(2.0f);
    telemetry_init();

    ESP_LOGI(TAG, "Tracker System Ready");

    while (1) {
        int64_t current_time = esp_timer_get_time();
        float dt = (float)(current_time - last_time) / 1000000.0f;
        last_time = current_time;

        // 6. Acquisition (Corrected)
        icm20948_read_accel_corrected(imu, &tracker.accel);
        icm20948_read_gyro_corrected(imu, &tracker.gyro);

        // 7. Processing
        ahrs_update(tracker.accel, tracker.gyro, dt, &tracker.attitude);
        ahrs_process_motion(&tracker, dt);

        // 8. Communication & Logging
        telemetry_send_state(tracker);
        logger_log_data(tracker);

        ESP_LOGI(TAG, "R:%5.1f | P:%5.1f | X:%.1f Y:%.1f", 
                 tracker.attitude.roll, tracker.attitude.pitch, 
                 tracker.position.x, tracker.position.y);

        vTaskDelay(pdMS_TO_TICKS(10)); 
    }
}

}
