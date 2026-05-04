#ifndef TELEMETRY_H
#define TELEMETRY_H

#include "tracker_types.h"
#include "esp_err.h"

/**
 * @brief Initialize telemetry modules (LoRa)
 */
void telemetry_init(void);

/**
 * @brief Send state data to ground station
 */
void telemetry_send_state(tracker_state_t state);

/**
 * @brief Receive state data from tracker
 * @param state Pointer to store the received state
 * @param timeout_ms Timeout for reception
 * @return ESP_OK if packet received successfully
 */
esp_err_t telemetry_receive(tracker_state_t *state, uint32_t timeout_ms);

#endif // TELEMETRY_H
