#include "telemetry.h"
#include "esp_log.h"
#include "driver/spi_master.h"
#include "bus_manager.h"
#include <string.h>

static const char *TAG = "TELEMETRY";
static spi_device_handle_t lora_spi;

/**
 * @brief Compact telemetry packet format
 */
typedef struct __attribute__((packed)) {
    uint32_t timestamp;
    float roll;
    float pitch;
    float yaw;
    int16_t accel_x;
    int16_t accel_y;
    int16_t accel_z;
} telemetry_packet_t;

void telemetry_init(void)
{
    spi_device_interface_config_t devcfg = {
        .clock_speed_hz = 10 * 1000 * 1000,      // 10 MHz
        .mode = 0,                               // SPI mode 0
        .spics_io_num = SPI_LORA_CS_IO,
        .queue_size = 7,
    };

    esp_err_t ret = spi_bus_add_device(SPI_LORA_HOST, &devcfg, &lora_spi);
    if (ret != ESP_OK) {
        ESP_LOGE(TAG, "Failed to add LoRa SPI device");
        return;
    }

    // Placeholder: Initialize SX127x registers here
    // (e.g., set frequency to 433MHz/868MHz/915MHz, SF, BW, etc.)
    
    ESP_LOGI(TAG, "LoRa SPI device added, ready for config");
}

void telemetry_send_state(tracker_state_t state)
{
    telemetry_packet_t packet;
    
    // Fill packet with compact data
    packet.timestamp = esp_log_timestamp();
    packet.roll = state.attitude.roll;
    packet.pitch = state.attitude.pitch;
    packet.yaw = state.attitude.yaw;
    packet.accel_x = (int16_t)state.accel.x;
    packet.accel_y = (int16_t)state.accel.y;
    packet.accel_z = (int16_t)state.accel.z;

    // Placeholder: Send packet via LoRa
    ESP_LOGD(TAG, "Telemetry Packet Sent (%d bytes)", sizeof(packet));
}

esp_err_t telemetry_receive(tracker_state_t *state, uint32_t timeout_ms)
{
    telemetry_packet_t packet;
    
    // Placeholder: Receive packet from LoRa
    // In a real implementation, you would:
    // 1. Wait for RX_DONE interrupt or poll
    // 2. Read packet from SX127x FIFO
    // 3. Verify CRC
    
    // For now, return ESP_ERR_TIMEOUT to simulate waiting for a packet
    // Once hardware is connected, actual LoRa SPI reads will go here
    return ESP_ERR_TIMEOUT; 

    /* 
    // Logic for decoding when data is actually received:
    state->attitude.roll = packet.roll;
    state->attitude.pitch = packet.pitch;
    state->attitude.yaw = packet.yaw;
    state->accel.x = (float)packet.accel_x;
    state->accel.y = (float)packet.accel_y;
    state->accel.z = (float)packet.accel_z;
    return ESP_OK;
    */
}
