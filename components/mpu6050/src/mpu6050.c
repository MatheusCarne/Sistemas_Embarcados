#include <stdio.h>
#include "mpu6050.h"
#include "driver/i2c.h"
#include "esp_log.h"

static const char *TAG = "MPU6050";

// Variável para armazenar a configuração
static mpu6050_config_t mpu_config;

// Inicialização do driver I2C
static esp_err_t i2c_master_init() {
    i2c_config_t conf = {
        .mode = I2C_MODE_MASTER,
        .sda_io_num = mpu_config.sda_pin,
        .scl_io_num = mpu_config.scl_pin,
        .sda_pullup_en = GPIO_PULLUP_ENABLE,
        .scl_pullup_en = GPIO_PULLUP_ENABLE,
        .master.clk_speed = mpu_config.i2c_freq,
    };
    ESP_ERROR_CHECK(i2c_param_config(mpu_config.i2c_port, &conf));
    return i2c_driver_install(mpu_config.i2c_port, conf.mode, 0, 0, 0);
}

// Escrever no registrador
static esp_err_t mpu6050_write_byte(uint8_t reg_addr, uint8_t data) {
    i2c_cmd_handle_t cmd = i2c_cmd_link_create();
    i2c_master_start(cmd);
    i2c_master_write_byte(cmd, (MPU6050_ADDR << 1) | I2C_MASTER_WRITE, true);
    i2c_master_write_byte(cmd, reg_addr, true);
    i2c_master_write_byte(cmd, data, true);
    i2c_master_stop(cmd);
    esp_err_t ret = i2c_master_cmd_begin(mpu_config.i2c_port, cmd, pdMS_TO_TICKS(1000));
    i2c_cmd_link_delete(cmd);
    return ret;
}

// Ler de um registrador
static esp_err_t mpu6050_read_bytes(uint8_t reg_addr, uint8_t *data, size_t len) {
    i2c_cmd_handle_t cmd = i2c_cmd_link_create();
    i2c_master_start(cmd);
    i2c_master_write_byte(cmd, (MPU6050_ADDR << 1) | I2C_MASTER_WRITE, true);
    i2c_master_write_byte(cmd, reg_addr, true);
    i2c_master_start(cmd);
    i2c_master_write_byte(cmd, (MPU6050_ADDR << 1) | I2C_MASTER_READ, true);
    i2c_master_read(cmd, data, len, I2C_MASTER_LAST_NACK);
    i2c_master_stop(cmd);
    esp_err_t ret = i2c_master_cmd_begin(mpu_config.i2c_port, cmd, pdMS_TO_TICKS(1000));
    i2c_cmd_link_delete(cmd);
    return ret;
}

// Inicialização do MPU6050
esp_err_t mpu6050_init(const mpu6050_config_t *config) {
    if (!config) return ESP_ERR_INVALID_ARG;

    mpu_config = *config;

    ESP_ERROR_CHECK(i2c_master_init());

    // Sair do modo de suspensão
    ESP_ERROR_CHECK(mpu6050_write_byte(PWR_MGMT_1_REG, 0x00));

    uint8_t who_am_i = 0;
    ESP_ERROR_CHECK(mpu6050_read_bytes(WHO_AM_I_REG, &who_am_i, 1));
    if (who_am_i != 0x68) {
        ESP_LOGE(TAG, "Falha ao reconhecer o MPU6050. ID: 0x%02X", who_am_i);
        return ESP_FAIL;
    }

    ESP_LOGI(TAG, "MPU6050 inicializado com sucesso!");
    return ESP_OK;
}

// Ler dados do sensor
esp_err_t mpu6050_read_data(mpu6050_data_t *data) {
    if (!data) return ESP_ERR_INVALID_ARG;

    uint8_t buffer[14];
    ESP_ERROR_CHECK(mpu6050_read_bytes(ACCEL_XOUT_H, buffer, 14));

    data->accel_x = (int16_t)((buffer[0] << 8) | buffer[1]) / 16384.0;
    data->accel_y = (int16_t)((buffer[2] << 8) | buffer[3]) / 16384.0;
    data->accel_z = (int16_t)((buffer[4] << 8) | buffer[5]) / 16384.0;
    data->gyro_x = (int16_t)((buffer[8] << 8) | buffer[9]) / 131.0;
    data->gyro_y = (int16_t)((buffer[10] << 8) | buffer[11]) / 131.0;
    data->gyro_z = (int16_t)((buffer[12] << 8) | buffer[13]) / 131.0;

    return ESP_OK;
}
