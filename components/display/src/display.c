#include "display.h"
#include "esp_log.h"
#include "bus_manager.h"
#include <string.h>

static const char *TAG = "DISPLAY";

// Typical SSD1306 I2C address
#define OLED_I2C_ADDRESS   0x3C

esp_err_t display_init(void)
{
    // In a real implementation, you would send the SSD1306 init sequence here
    // e.g., Set multiplex ratio, Display offset, Start line, Charge pump, etc.
    
    ESP_LOGI(TAG, "OLED Display Initialized (I2C: 0x%02X)", OLED_I2C_ADDRESS);
    return ESP_OK;
}

void display_update_tracker_data(tracker_state_t state)
{
    // This is where you would format and send text/graphics to the OLED
    // For now, we'll log what would be shown on screen
    ESP_LOGD(TAG, "OLED Update: R:%.1f P:%.1f Y:%.1f", 
             state.attitude.roll, state.attitude.pitch, state.attitude.yaw);
    
    /*
    ssd1306_clear_screen();
    ssd1306_draw_string(0, 0, "RC TRACKER LIVE");
    ssd1306_draw_string(0, 16, "ROLL: %.1f", state.attitude.roll);
    ssd1306_draw_string(0, 32, "PITCH: %.1f", state.attitude.pitch);
    ssd1306_refresh();
    */
}

void display_show_waiting(void)
{
    ESP_LOGI(TAG, "OLED: Waiting for Signal...");
    // ssd1306_draw_string(0, 0, "SEARCHING...");
}
