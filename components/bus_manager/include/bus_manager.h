#ifndef BUS_MANAGER_H
#define BUS_MANAGER_H

#include "esp_err.h"
#include "driver/i2c.h"
#include "driver/spi_master.h"

// I2C Configuration
#define I2C_MASTER_SCL_IO           22
#define I2C_MASTER_SDA_IO           21
#define I2C_MASTER_NUM              I2C_NUM_0
#define I2C_MASTER_FREQ_HZ          400000

// SPI Configuration (for LoRa and SD)
#define SPI_BUS_HOST                SPI2_HOST
#define SPI_LORA_CS_IO              5
#define SPI_SD_CS_IO                4
#define SPI_MOSI_IO                 23
#define SPI_MISO_IO                 19
#define SPI_SCLK_IO                 18

/**
 * @brief Initialize all system communication buses (I2C, SPI)
 * @return ESP_OK on success
 */
esp_err_t bus_manager_init_all(void);

#endif // BUS_MANAGER_H
