/*
 * mpu6050.h
 *
 *  Created on: Jul 27, 2025
 *      Author: tcast
 */

#ifndef IMU_H_
#define IMU_H_

#include "stm32f4xx_hal.h"
#include <stdint.h>

// I2C address for MPU6050 (left-shifted for HAL compatibility)
#define IMU_ADDR         (0x68 << 1)
#define IMU_PWR_MGMT_1   0x6B
#define IMU_ACCEL_XOUT_H 0x3B

// Struct to hold raw accelerometer and gyro data
typedef struct {
    int16_t Ax;
    int16_t Ay;
    int16_t Az;
    int16_t Gx;
    int16_t Gy;
    int16_t Gz;
} IMU_RawData;

// Function declarations
void IMU_Init(I2C_HandleTypeDef *hi2c);
HAL_StatusTypeDef IMU_ReadRaw(I2C_HandleTypeDef *hi2c, IMU_RawData *data);
void IMU_Filter_Update(IMU_RawData *data);

// Filtered output values (in degrees)
extern float imu_pitch;
extern float imu_roll;

#endif /* IMU_H_ */

