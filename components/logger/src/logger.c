#include "logger.h"
#include "esp_vfs_fat.h"
#include "sdmmc_cmd.h"
#include "bus_manager.h"
#include "esp_log.h"

static const char *TAG = "LOGGER";
static sdmmc_card_t *card;
static FILE *f;

#define MOUNT_POINT "/sdcard"

esp_err_t logger_init(void)
{
    ESP_LOGI(TAG, "Initializing SD card...");

    esp_vfs_fat_sdmmc_mount_config_t mount_config = {
        .format_if_mount_failed = true,
        .max_files = 5,
        .allocation_unit_size = 16 * 1024
    };

    sdmmc_host_t host = SDSPI_HOST_DEFAULT();
    host.slot = SPI_BUS_HOST;

    sdspi_device_config_t slot_config = SDSPI_DEVICE_CONFIG_DEFAULT();
    slot_config.gpio_cs = SPI_SD_CS_IO;
    slot_config.host_id = host.slot;

    esp_err_t ret = esp_vfs_fat_sdspi_mount(MOUNT_POINT, &host, &slot_config, &mount_config, &card);

    if (ret != ESP_OK) {
        ESP_LOGE(TAG, "Failed to mount filesystem.");
        return ret;
    }

    f = fopen(MOUNT_POINT"/flight.csv", "a");
    if (f == NULL) {
        ESP_LOGE(TAG, "Failed to open file for writing");
        return ESP_FAIL;
    }

    fprintf(f, "Time,Roll,Pitch,Yaw,AccX,AccY,AccZ\n");
    ESP_LOGI(TAG, "Logging started: "MOUNT_POINT"/flight.csv");

    return ESP_OK;
}

void logger_log_data(tracker_state_t state)
{
    if (f == NULL) return;

    fprintf(f, "%ld,%.2f,%.2f,%.2f,%.1f,%.1f,%.1f\n",
            esp_log_timestamp(),
            state.attitude.roll, state.attitude.pitch, state.attitude.yaw,
            state.accel.x, state.accel.y, state.accel.z);
    
    // Periodically flush to ensure data isn't lost on power cut
    static int flush_counter = 0;
    if (++flush_counter > 50) {
        fflush(f);
        flush_counter = 0;
    }
}

void logger_finish(void)
{
    if (f) fclose(f);
    esp_vfs_fat_sdcard_unmount(MOUNT_POINT, card);
}
