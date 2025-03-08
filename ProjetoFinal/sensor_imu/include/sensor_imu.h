#ifndef SENSOR_IMU_H
#define SENSOR_IMU_H

#include <esp_err.h>
#include "driver/gpio.h"
#include <math.h>

//Configurações do MPU.

#define MPU6050_PWR_MGMT_1_REG   0x6B
#define MPU6050_PWR_MGMT_1_CLKSEL 0x01 
#define I2C_MASTER_FREQ_HZ      100000           /**< Frequência do barramento I2C. */
#define I2C_MASTER_NUM          I2C_NUM_0        /**< Número do barramento I2C. */
#define MPU6050_ADDR            0x68
#define MPU6050_WHO_AM_I_REG    0x75
#define MPU6050_ACCEL_XOUT_H    0x3B

//Constantes de conversão e fatores de escala.

#define GYRO_SCALE             65.5              /**< Fator de escala para ±250°/s. */
#define ACCEL_SCALE            16384.0            /**< Fator de escala para ±2g. */
#define RAD_PER_DEG            (3.14159265358979 / 180.0) /**< Conversão de graus para radianos. */
#define RAD_TO_DEG             (180.0 / M_PI)       /**< Conversão de radianos para graus. */

//Estrutura para dados de aceleração.

typedef struct {
    float x, y, z;
} AccelerationData;

//Estrutura para dados de giroscópio.

typedef struct {
    float x, y, z;
} GyroscopeData;

esp_err_t imu_init(uint8_t devAddr, gpio_num_t sda_pin, gpio_num_t scl_pin);
esp_err_t imu_get_acceleration_data(AccelerationData *data);
esp_err_t imu_get_gyroscope_data(GyroscopeData *data);
esp_err_t imu_deinit();

#endif
