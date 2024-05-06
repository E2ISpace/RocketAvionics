#ifndef complementary_h
#define complementary_h

//https://ahrs.readthedocs.io/en/latest/filters/complementary.html

typedef struct 
{
    /* data */
    float accel[3];
    float gyro[3];
    float mag[3];

}IMU;

typedef struct 
{
    /* data */
    float deg_yaw;
    float deg_pitch;
    float deg_roll;

    float rad_yaw;
    float rad_pitch;
    float rad_roll;

}complementary;

bool complementaryInit(void);
void complementaryUpdate(IMU imu);
bool complementaryGetInfo(complementary *p_info);

#endif