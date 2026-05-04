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
    int16_t posX; // Position in cm (to save space, scaled from float)
    int16_t posY;
    int16_t posZ;
} telemetry_packet_t;

void telemetry_init(void)
{
    spi_device_interface_config_t devcfg = {
        .clock_speed_hz = 10 * 1000 * 1000,      // 10 MHz
        .mode = 0,                               // SPI mode 0
        .spics_io_num = SPI_LORA_CS_IO,
        .queue_size = 7,
    };

    esp_err_t ret = spi_bus_add_device(SPI_BUS_HOST, &devcfg, &lora_spi);
    if (ret != ESP_OK) {
        ESP_LOGE(TAG, "Failed to add LoRa SPI device");
        return;
    }

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
    
    // Scale position to cm for transmission
    packet.posX = (int16_t)(state.position.x * 100.0f);
    packet.posY = (int16_t)(state.position.y * 100.0f);
    packet.posZ = (int16_t)(state.position.z * 100.0f);

    // Placeholder: Send packet via LoRa
    ESP_LOGD(TAG, "Telemetry Packet Sent (%d bytes)", sizeof(packet));
}

esp_err_t telemetry_receive(tracker_state_t *state, uint32_t timeout_ms)
{
    // Simulation: in real use, this would read from SPI
    return ESP_ERR_TIMEOUT; 

    /* 
    // Logic for decoding when data is actually received:
    telemetry_packet_t packet;
    state->attitude.roll = packet.roll;
    state->attitude.pitch = packet.pitch;
    state->attitude.yaw = packet.yaw;
    state->position.x = (float)packet.posX / 100.0f;
    state->position.y = (float)packet.posY / 100.0f;
    state->position.z = (float)packet.posZ / 100.0f;
    return ESP_OK;
    */
}
