#ifndef TRACKER_TYPES_H
#define TRACKER_TYPES_H

/**
 * @brief Common 3-axis vector for raw or processed data
 */
typedef struct {
    float x;
    float y;
    float z;
} vector3_t;

/**
 * @brief Euler angles for orientation
 */
typedef struct {
    float roll;
    float pitch;
    float yaw;
} attitude_t;

/**
 * @brief Complete state of the tracker
 */
typedef struct {
    vector3_t accel;
    vector3_t gyro;
    vector3_t mag;
    attitude_t attitude;
} tracker_state_t;

#endif // TRACKER_TYPES_H
