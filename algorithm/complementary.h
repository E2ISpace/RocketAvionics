#ifndef complementary_h
#define complementary_h

//https://ahrs.readthedocs.io/en/latest/filters/complementary.html

typedef struct 
{
    /* data */
    float ax,ay,az;
    float gx,gy,gz;
    float mx,my,mz;

    float dt;

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
float invSqrt(float x);

#endif