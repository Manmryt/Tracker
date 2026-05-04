#ifndef LOGGER_H
#define LOGGER_H

#include "tracker_types.h"
#include "esp_err.h"

/**
 * @brief Initialize the SD card logger
 * @return ESP_OK on success
 */
esp_err_t logger_init(void);

/**
 * @brief Log current tracker state to the SD card
 * @param state The state to log
 */
void logger_log_data(tracker_state_t state);

/**
 * @brief Gracefully close the log file
 */
void logger_finish(void);

#endif // LOGGER_H
