#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_log.h"

#include "bus_manager.h"
#include "telemetry.h"
#include "display.h"
#include "tracker_types.h"

static const char *TAG = "GROUND_STATION";

void app_main(void)
{
    tracker_state_t tracker_data = {0};

    ESP_LOGI(TAG, "Starting Ground Station Receiver...");

    // 1. Initialize Communication Buses (I2C for OLED, SPI for LoRa)
    ESP_ERROR_CHECK(bus_manager_init_all());

    // 2. Initialize Hardware Components
    display_init();

    // 3. Initialize Telemetry Layer
    telemetry_init();

    ESP_LOGI(TAG, "Ground Station Ready. Listening for Tracker...");
    display_show_waiting();

    while (1) {
        // 4. Listen for incoming telemetry packets
        esp_err_t ret = telemetry_receive(&tracker_data, 1000);

        if (ret == ESP_OK) {
            // 5. Update display with live data
            display_update_tracker_data(tracker_data);
            
            ESP_LOGI(TAG, "RECEIVE [Roll: %5.1f | Pitch: %5.1f]",
                     tracker_data.attitude.roll, 
                     tracker_data.attitude.pitch);
        } else if (ret == ESP_ERR_TIMEOUT) {
            // No data received
            display_show_waiting();
            ESP_LOGW(TAG, "Signal Lost. Waiting...");
        }

        vTaskDelay(pdMS_TO_TICKS(10)); 
    }
}
