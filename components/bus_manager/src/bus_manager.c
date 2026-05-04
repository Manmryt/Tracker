#include "bus_manager.h"
#include "esp_log.h"

static const char *TAG = "BUS_MANAGER";

static esp_err_t i2c_master_init(void)
{
    i2c_config_t conf = {
        .mode = I2C_MODE_MASTER,
        .sda_io_num = I2C_MASTER_SDA_IO,
        .scl_io_num = I2C_MASTER_SCL_IO,
        .sda_pullup_en = GPIO_PULLUP_ENABLE,
        .scl_pullup_en = GPIO_PULLUP_ENABLE,
        .master.clk_speed = I2C_MASTER_FREQ_HZ,
    };

    esp_err_t err = i2c_param_config(I2C_MASTER_NUM, &conf);
    if (err != ESP_OK) return err;

    return i2c_driver_install(I2C_MASTER_NUM, conf.mode, 0, 0, 0);
}

static esp_err_t spi_master_init(void)
{
    spi_bus_config_t buscfg = {
        .miso_io_num = SPI_MISO_IO,
        .mosi_io_num = SPI_MOSI_IO,
        .sclk_io_num = SPI_SCLK_IO,
        .quadwp_io_num = -1,
        .quadhd_io_num = -1,
        .max_transfer_sz = 4000,
    };

    return spi_bus_initialize(SPI_BUS_HOST, &buscfg, SPI_DMA_CH_AUTO);
}

esp_err_t bus_manager_init_all(void)
{
    esp_err_t err;

    err = i2c_master_init();
    if (err != ESP_OK) {
        ESP_LOGE(TAG, "Failed to initialize I2C");
        return err;
    }
    ESP_LOGI(TAG, "I2C Bus initialized");

    err = spi_master_init();
    if (err != ESP_OK) {
        ESP_LOGE(TAG, "Failed to initialize SPI");
        return err;
    }
    ESP_LOGI(TAG, "SPI Bus initialized");
    
    return ESP_OK;
}
