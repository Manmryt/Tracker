#ifndef AHRS_H
#define AHRS_H

#include "tracker_types.h"

/**
 * @brief Initialize the AHRS system with filter parameters
 * @param beta Filter gain
 */
void ahrs_init(float beta);

/**
 * @brief Update the orientation estimate
 * @param accel Raw accelerometer data (m/s^2 or g)
 * @param gyro Raw gyroscope data (rad/s or deg/s)
 * @param dt Time step in seconds
 * @param attitude Output orientation estimate
 */
void ahrs_update(vector3_t accel, vector3_t gyro, float dt, attitude_t *attitude);

#endif // AHRS_H
