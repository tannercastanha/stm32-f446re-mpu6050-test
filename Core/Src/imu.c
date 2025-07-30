/*
 * mpu6050.c
 *
 *  Created on: Jul 27, 2025
 *      Author: tcast
 */
#include "imu.h"
#include <math.h>
#include <stdio.h>

float imu_pitch = 0.0f;
float imu_roll = 0.0f;

static float alpha = 0.98f;  // Complementary filter constant
static float dt = 0.005f;    // Sample interval = 5 ms

void IMU_Init(I2C_HandleTypeDef *hi2c) {
    uint8_t data = 0x00;
    HAL_I2C_Mem_Write(hi2c, IMU_ADDR, IMU_PWR_MGMT_1, 1, &data, 1, HAL_MAX_DELAY);
}

HAL_StatusTypeDef IMU_ReadRaw(I2C_HandleTypeDef *hi2c, IMU_RawData *data) {
    uint8_t buf[14];
    HAL_StatusTypeDef ret = HAL_I2C_Mem_Read(hi2c, IMU_ADDR, IMU_ACCEL_XOUT_H, 1, buf, 14, HAL_MAX_DELAY);
    if (ret != HAL_OK) return ret;

    data->Ax = (int16_t)(buf[0] << 8 | buf[1]);
    data->Ay = (int16_t)(buf[2] << 8 | buf[3]);
    data->Az = (int16_t)(buf[4] << 8 | buf[5]);
    data->Gx = (int16_t)(buf[8] << 8 | buf[9]);
    data->Gy = (int16_t)(buf[10] << 8 | buf[11]);
    data->Gz = (int16_t)(buf[12] << 8 | buf[13]);

    return HAL_OK;
}

void IMU_Filter_Update(IMU_RawData *data) {
    float Ax = (float)data->Ax / 16384.0f;
    float Ay = (float)data->Ay / 16384.0f;
    float Az = (float)data->Az / 16384.0f;
    float Gx = (float)data->Gx / 131.0f;  // deg/s
    float Gy = (float)data->Gy / 131.0f;

    float acc_pitch = atan2f(Ay, sqrtf(Ax * Ax + Az * Az)) * 180.0f / 3.14159f;
    float acc_roll  = atan2f(-Ax, sqrtf(Ay * Ay + Az * Az)) * 180.0f / 3.14159f;

    imu_pitch = alpha * (imu_pitch + Gy * dt) + (1 - alpha) * acc_pitch;
    imu_roll  = alpha * (imu_roll  + Gx * dt) + (1 - alpha) * acc_roll;
}



