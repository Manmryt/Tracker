#include <stdlib.h>
#include <string.h>
#include "icm20948.h"
#include "esp_log.h"

static const char *TAG = "ICM20948";

struct icm20948_dev_t {
    i2c_port_t port;
    uint16_t addr;
};

static esp_err_t write_reg(icm20948_handle_t dev, uint8_t reg, uint8_t data)
{
    uint8_t write_buf[2] = {reg, data};
    return i2c_master_write_to_device(dev->port, dev->addr, write_buf, sizeof(write_buf), pdMS_TO_TICKS(100));
}

static esp_err_t read_reg(icm20948_handle_t dev, uint8_t reg, uint8_t *data, size_t len)
{
    return i2c_master_write_read_device(dev->port, dev->addr, &reg, 1, data, len, pdMS_TO_TICKS(100));
}

static esp_err_t select_bank(icm20948_handle_t dev, uint8_t bank)
{
    if (bank > 3) return ESP_ERR_INVALID_ARG;
    return write_reg(dev, REG_BANK_SEL, bank << 4);
}

icm20948_handle_t icm20948_create(i2c_port_t port, uint16_t addr)
{
    icm20948_handle_t dev = (icm20948_handle_t)calloc(1, sizeof(struct icm20948_dev_t));
    if (dev) {
        dev->port = port;
        dev->addr = addr;
    }
    return dev;
}

esp_err_t icm20948_init(icm20948_handle_t dev)
{
    uint8_t who_am_i;
    esp_err_t ret;

    ret = select_bank(dev, 0);
    if (ret != ESP_OK) return ret;

    ret = read_reg(dev, REG_WHO_AM_I, &who_am_i, 1);
    if (ret != ESP_OK) return ret;

    if (who_am_i != ICM20948_WHO_AM_I_VAL) {
        ESP_LOGE(TAG, "Unexpected WHO_AM_I value: 0x%02X", who_am_i);
        return ESP_FAIL;
    }

    write_reg(dev, REG_PWR_MGMT_1, 0x80); // Reset
    vTaskDelay(pdMS_TO_TICKS(100));
    write_reg(dev, REG_PWR_MGMT_1, 0x01); // Wake
    
    ESP_LOGI(TAG, "ICM20948 initialized");
    return ESP_OK;
}

esp_err_t icm20948_read_accel(icm20948_handle_t dev, vector3_t *accel)
{
    uint8_t data[6];
    esp_err_t ret = read_reg(dev, REG_ACCEL_XOUT_H, data, 6);
    if (ret != ESP_OK) return ret;

    accel->x = (int16_t)((data[0] << 8) | data[1]);
    accel->y = (int16_t)((data[2] << 8) | data[3]);
    accel->z = (int16_t)((data[4] << 8) | data[5]);

    return ESP_OK;
}

esp_err_t icm20948_read_gyro(icm20948_handle_t dev, vector3_t *gyro)
{
    uint8_t data[6];
    esp_err_t ret = read_reg(dev, REG_GYRO_XOUT_H, data, 6);
    if (ret != ESP_OK) return ret;

    gyro->x = (int16_t)((data[0] << 8) | data[1]);
    gyro->y = (int16_t)((data[2] << 8) | data[3]);
    gyro->z = (int16_t)((data[4] << 8) | data[5]);

    return ESP_OK;
}
