#ifndef DISPLAY_H
#define DISPLAY_H

#include "tracker_types.h"
#include "esp_err.h"

/**
 * @brief Initialize the OLED display
 * @return ESP_OK on success
 */
esp_err_t display_init(void);

/**
 * @brief Update the display with current tracker orientation
 * @param state The current tracker state to display
 */
void display_update_tracker_data(tracker_state_t state);

/**
 * @brief Show a "Signal Lost" or "Waiting" message
 */
void display_show_waiting(void);

#endif // DISPLAY_H
