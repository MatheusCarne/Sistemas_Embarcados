#ifndef MPU6050_H
#define MPU6050_H

#include "esp_err.h"

// Definições para o MPU6050
#define MPU6050_ADDR 0x68
#define WHO_AM_I_REG 0x75
#define PWR_MGMT_1_REG 0x6B
#define ACCEL_XOUT_H 0x3B

// Estrutura de configuração do MPU6050
typedef struct {
    int sda_pin;
    int scl_pin;
    int i2c_port;
    uint32_t i2c_freq;
} mpu6050_config_t;

// Estrutura para armazenar leituras do sensor
typedef struct {
    float accel_x;
    float accel_y;
    float accel_z;
    float gyro_x;
    float gyro_y;
    float gyro_z;
} mpu6050_data_t;

// Inicialização do sensor
esp_err_t mpu6050_init(const mpu6050_config_t *config);

// Ler dados do sensor
esp_err_t mpu6050_read_data(mpu6050_data_t *data);

#endif // MPU6050_H
