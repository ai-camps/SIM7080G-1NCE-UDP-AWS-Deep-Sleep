#ifndef MPU_Read_H
#define MPU_Read_H

#include "Adafruit_MPU6050.h"

extern Adafruit_MPU6050 mpu;

sensors_event_t accel, gyro, temp;

struct MPUData
{
    float ax;
    float ay;
    float az;
    float gx;
    float gy;
    float gz;
    float tempF;
};

// Define the function to initialize and read current value from the vibration and MPU6050 sensors
MPUData MPU_Read(MPUData &mpudata)
{
    Serial.println("..............................................");
    Serial.println("Reading the value of motion and temperature from MPU6050 ... ");

    // Read the accelerometer, gyroscope and temperature values from MPU6050
    mpu.getEvent(&accel, &gyro, &temp);

    // Convert the value to float
    mpudata.ax = accel.acceleration.x;
    mpudata.ay = accel.acceleration.y;
    mpudata.az = accel.acceleration.z;
    mpudata.gx = gyro.gyro.x;
    mpudata.gy = gyro.gyro.y;
    mpudata.gz = gyro.gyro.z;
    mpudata.tempF = temp.temperature * 1.8 + 32.;

    // Print the current readings from the MPU6050
    Serial.println("Current values are:  " + String(mpudata.ax) + " " + String(mpudata.ay) + " " + String(mpudata.az) + " " + String(mpudata.gx) + " " + String(mpudata.gy) + " " + String(mpudata.gz) + " " + String(mpudata.tempF));

    return mpudata;
}

#endif
