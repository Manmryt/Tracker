#ifndef ICM20948_H
#define ICM20948_H

#include "driver/i2c.h"
#include "esp_err.h"
#include "tracker_types.h"

#define ICM20948_I2C_ADDR          0x68   
#define ICM20948_WHO_AM_I_VAL      0xEA   

// Register Addresses (Bank 0)
#define REG_WHO_AM_I               0x00
#define REG_USER_CTRL              0x03
#define REG_LP_CONFIG              0x05
#define REG_PWR_MGMT_1             0x06
#define REG_PWR_MGMT_2             0x07
#define REG_INT_PIN_CFG            0x0F

#define REG_ACCEL_XOUT_H           0x2D
#define REG_GYRO_XOUT_H            0x33

#define REG_BANK_SEL               0x7F

typedef struct icm20948_dev_t *icm20948_handle_t;

/**
 * @brief Create an ICM20948 device handle
 */
icm20948_handle_t icm20948_create(i2c_port_t port, uint16_t addr);

/**
 * @brief Initialize the ICM20948 sensor
 */
esp_err_t icm20948_init(icm20948_handle_t dev);

/**
 * @brief Offset values for sensor calibration
 */
typedef struct {
    vector3_t accel_offset;
    vector3_t gyro_offset;
} icm20948_offsets_t;

/**
 * @brief Run a calibration routine to calculate sensor offsets
 * @note Device must be kept perfectly still and level during this process
 * @param dev Device handle
 * @param offsets Output structure to store calculated offsets
 * @return ESP_OK on success
 */
esp_err_t icm20948_calibrate(icm20948_handle_t dev, icm20948_offsets_t *offsets);

/**
 * @brief Apply previously calculated offsets to the device handle
 * @param dev Device handle
 * @param offsets Offsets to apply
 */
void icm20948_apply_offsets(icm20948_handle_t dev, icm20948_offsets_t offsets);

/**
 * @brief Read corrected accelerometer data (offset applied)
 */
esp_err_t icm20948_read_accel_corrected(icm20948_handle_t dev, vector3_t *accel);

/**
 * @brief Read corrected gyroscope data (offset applied)
 */
esp_err_t icm20948_read_gyro_corrected(icm20948_handle_t dev, vector3_t *gyro);

#endif 
