#include <stdio.h>
#include "sensor_imu.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/i2c.h"
#include "esp_log.h"
#include <esp_err.h>

#define MPU6050_ADDR            0x68             /**< Endereço padrão do MPU6050 */

/**
 * @brief Inicializa o sensor IMU.
 *
 * @param devAddr Endereço do dispositivo I2C.
 * @param sda_pin GPIO para SDA.
 * @param scl_pin GPIO para SCL.
 * @return esp_err_t ESP_OK em caso de sucesso, ou ESP_FAIL em caso de falha.
 */
esp_err_t imu_init(uint8_t devAddr, gpio_num_t sda_pin, gpio_num_t scl_pin)
{
    esp_err_t ret;

    i2c_config_t conf = {
        .mode = I2C_MODE_MASTER,
        .sda_io_num = sda_pin,
        .scl_io_num = scl_pin,
        .sda_pullup_en = GPIO_PULLUP_ENABLE,
        .scl_pullup_en = GPIO_PULLUP_ENABLE,
        .master.clk_speed = I2C_MASTER_FREQ_HZ,
    };

    ret = i2c_param_config(I2C_MASTER_NUM, &conf);
    if (ret != ESP_OK) {
        printf("Failed to configure I2C parameters\n");
        return ESP_ERR_NOT_FOUND;
    }

    ret = i2c_driver_install(I2C_MASTER_NUM, I2C_MODE_MASTER, 0, 0, 0);
    if (ret != ESP_OK) {
        printf("Failed to install I2C driver\n");
        return ESP_ERR_NOT_FOUND;
    }

    uint8_t who_am_i = 0;
    i2c_cmd_handle_t cmd = i2c_cmd_link_create();
    i2c_master_start(cmd);
    i2c_master_write_byte(cmd, (devAddr << 1) | I2C_MASTER_WRITE, true);
    i2c_master_write_byte(cmd, MPU6050_WHO_AM_I_REG, true);
    i2c_master_start(cmd);
    i2c_master_write_byte(cmd, (devAddr << 1) | I2C_MASTER_READ, true);
    i2c_master_read_byte(cmd, &who_am_i, I2C_MASTER_LAST_NACK);
    i2c_master_stop(cmd);
    ret = i2c_master_cmd_begin(I2C_MASTER_NUM, cmd, pdMS_TO_TICKS(1000));
    i2c_cmd_link_delete(cmd);

    if (ret == ESP_OK && who_am_i == 0x68) {
        printf("MPU6050 found and initialized\n");
        return ESP_OK;
    } else {
        printf("MPU6050 not found or initialization failed\n");
        return ESP_ERR_NOT_FOUND;
    }
}

/**
 * @brief Lê os dados de aceleração do sensor IMU.
 *
 * @param data Ponteiro para estrutura AccelerationData onde os dados serão armazenados.
 * @return esp_err_t ESP_OK em caso de sucesso, ou ESP_FAIL em caso de falha.
 */
esp_err_t imu_get_acceleration_data(AccelerationData *data)
{
    if (data == NULL) {
        return ESP_FAIL;
    }
    esp_err_t ret;
    uint8_t accel_data[6];
    i2c_cmd_handle_t cmd = i2c_cmd_link_create();

    i2c_master_start(cmd);
    i2c_master_write_byte(cmd, (MPU6050_ADDR << 1) | I2C_MASTER_WRITE, true);
    i2c_master_write_byte(cmd, MPU6050_ACCEL_XOUT_H, true);
    i2c_master_start(cmd);
    i2c_master_write_byte(cmd, (MPU6050_ADDR << 1) | I2C_MASTER_READ, true);
    i2c_master_read(cmd, accel_data, sizeof(accel_data), I2C_MASTER_LAST_NACK);
    i2c_master_stop(cmd);

    ret = i2c_master_cmd_begin(I2C_MASTER_NUM, cmd, pdMS_TO_TICKS(1000));
    i2c_cmd_link_delete(cmd);

    if (ret != ESP_OK) {
      printf("Failed to read acceleration data\n");
      return ESP_FAIL;
    }

    int16_t raw_accel_x = (int16_t)((accel_data[0] << 8) | accel_data[1]);
    int16_t raw_accel_y = (int16_t)((accel_data[2] << 8) | accel_data[3]);
    int16_t raw_accel_z = (int16_t)((accel_data[4] << 8) | accel_data[5]);

    data->x = raw_accel_x / ACCEL_SCALE * 9.81; // Convertendo para m/s²
    data->y = raw_accel_y / ACCEL_SCALE * 9.81; // Convertendo para m/s²
    data->z = raw_accel_z / ACCEL_SCALE * 9.81; // Convertendo para m/s²

    data->x = data->x/9.80665; // Convertendo para g
    data->y = data->y/9.80665; // Convertendo para g
    data->z = data->z/9.80665; // Convertendo para g

    return ESP_OK;
}

/**
 * @brief Lê os dados do giroscópio do sensor IMU.
 *
 * @param data Ponteiro para estrutura GyroscopeData onde os dados serão armazenados.
 * @return esp_err_t ESP_OK em caso de sucesso, ou ESP_FAIL em caso de falha.
 */
esp_err_t imu_get_gyroscope_data(GyroscopeData *data)
{
   if (data == NULL) {
       return ESP_FAIL;
   }

   esp_err_t ret;
   uint8_t gyro_data[6];
   i2c_cmd_handle_t cmd = i2c_cmd_link_create();

   i2c_master_start(cmd);
   i2c_master_write_byte(cmd, (MPU6050_ADDR << 1) | I2C_MASTER_WRITE, true);
   i2c_master_write_byte(cmd, MPU6050_GYRO_XOUT_H, true);
   i2c_master_start(cmd);
   i2c_master_write_byte(cmd, (MPU6050_ADDR << 1) | I2C_MASTER_READ, true);
   i2c_master_read(cmd, gyro_data, sizeof(gyro_data), I2C_MASTER_LAST_NACK);
   i2c_master_stop(cmd);

   ret = i2c_master_cmd_begin(I2C_MASTER_NUM, cmd, pdMS_TO_TICKS(1000));
   i2c_cmd_link_delete(cmd);

   if (ret != ESP_OK) {
       printf("Failed to read gyroscope data\n");
       return ESP_FAIL;
   }

   int16_t raw_gyro_x = (int16_t)((gyro_data[0] << 8) | gyro_data[1]);
   int16_t raw_gyro_y = (int16_t)((gyro_data[2] << 8) | gyro_data[3]);
   int16_t raw_gyro_z = (int16_t)((gyro_data[4] << 8) | gyro_data[5]);

   data->x = raw_gyro_x / GYRO_SCALE * RAD_PER_DEG; // Convertendo para rad/s
   data->y = raw_gyro_y / GYRO_SCALE * RAD_PER_DEG; // Convertendo para rad/s
   data->z = raw_gyro_z / GYRO_SCALE * RAD_PER_DEG; // Convertendo para rad/s

   data->x *= RAD_TO_DEG;
   data->y *= RAD_TO_DEG;
   data->z *= RAD_TO_DEG;

   data->x /= 2;
   data->y /= 2;
   data->z /= 2;

   return ESP_OK;
}

/**
 * @brief Desinicializa o sensor IMU.
 *
 * @return esp_err_t ESP_OK em caso de sucesso, ou ESP_FAIL em caso de falha.
 */
esp_err_t imu_deinit()
{
    esp_err_t ret;

    ret = i2c_driver_delete(I2C_MASTER_NUM);
    if (ret != ESP_OK) {
        printf("Failed to uninstall I2C driver\n");
        return ESP_FAIL;
    }

    printf("I2C driver uninstalled successfully\n");
    return ESP_OK;
}
