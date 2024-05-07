#include "complementary.h"
#include <math.h>

#define ALPHA 0.9686

complementary com;

bool complementaryInit(void)
{
    com.deg_pitch = 0;
    com.deg_yaw = 0;
    com.deg_roll = 0;

    return true;
}

void complementaryUpdate(IMU imu)
{
    float atx, aty, atz; // accelerometer Theta X (roll). pitch yaw
    float gtx, gty, gtz; // gyro
    float bx, by, bz;    // to get atz

    atx = atan2f(imu.ay, imu.az);
    aty = atan2f(-imu.ax, sqrtf(imu.ay * imu.ay + imu.az * imu.az));
    bx = imu.mx * cosf(atx) + imu.my * sinf(atx) * sinf(aty) + imu.mz * sinf(atx) * cosf(aty);
    by = imu.my * cosf(aty) - imu.mz * sinf(aty);
    bz = -imu.mx * sinf(atx) + imu.my * cosf(atx) * sinf(aty) + imu.mz * cosf(atx) * cosf(aty);
    atz = atan2f(-by, bx);

    com.rad_pitch = atx;
    com.rad_roll = aty;
    com.rad_yaw = atz;
}

bool complementaryGetInfo(complementary *p_info)
{
    p_info->deg_roll = com.rad_pitch * 57.29578f;
    p_info->deg_pitch = com.rad_roll * 57.29578f;
    p_info->deg_yaw = com.rad_yaw * 57.29578f + 180.0f;
    return true;
}

float invSqrt(float x)
{
    float halfx = 0.5f * x;
    float y = x;
    long i = *(long *)&y;
    i = 0x5f3759df - (i >> 1);
    y = *(float *)&i;
    y = y * (1.5f - (halfx * y * y));
    y = y * (1.5f - (halfx * y * y));
    return y;
}