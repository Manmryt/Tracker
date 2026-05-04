#include "ahrs.h"
#include <math.h>

// Filter gains (tuned for tracker use-case)
static float kp = 2.0f;  // Proportional gain
static float ki = 0.0f;  // Integral gain (set to 0 to disable drift correction for now)

// Internal quaternion representing current orientation
static float q0 = 1.0f, q1 = 0.0f, q2 = 0.0f, q3 = 0.0f;
static float integralFBx = 0.0f, integralFBy = 0.0f, integralFBz = 0.0f;

void ahrs_init(float beta)
{
    // For Mahony, we use Kp instead of Beta, but we'll accept a param for consistency
    if (beta > 0) kp = beta;
}

void ahrs_update(vector3_t accel, vector3_t gyro, float dt, attitude_t *attitude)
{
    float recipNorm;
    float vx, vy, vz;
    float ex, ey, ez;
    float qa, qb, qc;

    // Convert degrees/sec to radians/sec for gyro
    float gx = gyro.x * (M_PI / 180.0f);
    float gy = gyro.y * (M_PI / 180.0f);
    float gz = gyro.z * (M_PI / 180.0f);

    // 1. Normalize accelerometer measurement
    recipNorm = 1.0f / sqrtf(accel.x * accel.x + accel.y * accel.y + accel.z * accel.z);
    float ax = accel.x * recipNorm;
    float ay = accel.y * recipNorm;
    float az = accel.z * recipNorm;

    // 2. Estimated direction of gravity (from quaternion)
    vx = 2.0f * (q1 * q3 - q0 * q2);
    vy = 2.0f * (q0 * q1 + q2 * q3);
    vz = q0 * q0 - q1 * q1 - q2 * q2 + q3 * q3;

    // 3. Error is cross product between estimated direction and measured direction of gravity
    ex = (ay * vz - az * vy);
    ey = (az * vx - ax * vz);
    ez = (ax * vy - ay * vx);

    // 4. Compute and apply integral feedback if enabled
    if (ki > 0.0f) {
        integralFBx += ex * ki * dt;
        integralFBy += ey * ki * dt;
        integralFBz += ez * ki * dt;
        gx += integralFBx;
        gy += integralFBy;
        gz += integralFBz;
    }

    // 5. Apply proportional feedback
    gx += kp * ex;
    gy += kp * ey;
    gz += kp * ez;

    // 6. Integrate rate of change of quaternion
    dt = 0.5f * dt;
    qa = q0; qb = q1; qc = q2;
    q0 += (-qb * gx - qc * gy - q3 * gz) * dt;
    q1 += (qa * gx + qc * gz - q3 * gy) * dt;
    q2 += (qa * gy - qb * gz + q3 * gx) * dt;
    q3 += (qa * gz + qb * gy - qc * gx) * dt;

    // 7. Normalize quaternion
    recipNorm = 1.0f / sqrtf(q0 * q0 + q1 * q1 + q2 * q2 + q3 * q3);
    q0 *= recipNorm;
    q1 *= recipNorm;
    q2 *= recipNorm;
    q3 *= recipNorm;

    // 8. Convert to Euler Angles (Roll, Pitch, Yaw)
    attitude->roll = atan2f(2.0f * (q0 * q1 + q2 * q3), q0 * q0 - q1 * q1 - q2 * q2 + q3 * q3) * 57.29578f;
    attitude->pitch = asinf(2.0f * (q0 * q2 - q1 * q3)) * 57.29578f;
    attitude->yaw = atan2f(2.0f * (q1 * q2 + q0 * q3), q0 * q0 + q1 * q1 - q2 * q2 - q3 * q3) * 57.29578f;
}
