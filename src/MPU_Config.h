#ifndef MPU_Config_H
#define MPU_Config_H

#include "Adafruit_MPU6050.h"
#include <Wire.h>

extern Adafruit_MPU6050 mpu;

extern int MPU_INT_PIN;

extern volatile bool motionDetected;

volatile bool motionDetected = false; // Add this line

// !This value indicates motion sensitivity about the minimum amount of motion required to trigger the motion detection interrupt on the MPU6050 sensor. The unit of the threshold value is milligravity (mg).
const uint16_t MPU_MOTION_THRESHOLD = 8; // ! set the motion threshold to 1mg, it can be 1mg, 2mg, 4mg, 8mg, 16mg, 32mg, 64mg or 128mg

// !This value indicates motion duration about the minimum amount of time that the acceleration on all axes must exceed the motion threshold before the motion interrupt is triggered on the MPU6050 sensor. The unit of the duration value is 1ms.
const uint16_t MPU_MOTION_DURATION = 20; // ! set the motion duration to 20ms, it can be 1ms, 20ms, 40ms, 80ms, 160ms, 320ms, 640ms or 1280ms

// ! Configure MPU registers
void MPU_Config()
{
    // Set parameters for the MPU6050 sensor
    mpu.setHighPassFilter(MPU6050_HIGHPASS_0_63_HZ);       // ! set the high pass filter to 0.63Hz, it can be 5Hz, 2.5Hz, 1.25Hz, 0.63Hz, 0.31Hz or 0.15Hz, its function is to eliminate the DC component of the acceleration signal
    mpu.setMotionDetectionThreshold(MPU_MOTION_THRESHOLD); // set the motion threshold to 1mg
    mpu.setMotionDetectionDuration(MPU_MOTION_DURATION);   // set the motion duration to 20ms
    mpu.setInterruptPinLatch(true);                        // ! set the interrupt pin to latch mode, it can be pulse mode or latch mode, the pulse mode is to generate a 50us long pulse, the latch mode is to keep the interrupt pin active until the interrupt status is cleared
    mpu.setInterruptPinPolarity(true);                     // ! set the interrupt pin to active high, it can be active high or active low, the active high is to set the interrupt pin to high when the interrupt is triggered, the active low is to set the interrupt pin to low when the interrupt is triggered
    mpu.setMotionInterrupt(true);                          // ! enable motion interrupt, it can be enabled or disabled, when the motion interrupt is enabled, the interrupt pin will be set to high when the motion is detected
    mpu.setCycleRate(MPU6050_CYCLE_5_HZ);                  // ! set the cycle rate to 5Hz, only works when the cycle mode is enabled as below
    mpu.enableCycle(false);                                // disable cycle mode
    mpu.setAccelerometerRange(MPU6050_ACC_RANGE_16G);      // set the accelerometer range to 2g, it can be 2g, 4g, 8g or 16g
    mpu.setGyroRange(MPU6050_GYR_RANGE_2000_DEG);          // set the gyroscope range to 250 deg/s, it can be 250, 500, 1000 or 2000 deg/s
}

void handleMotionInterrupt()
{
    motionDetected = true;
}

#endif
