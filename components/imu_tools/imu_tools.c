#include "imu_tools.h"
#include <stdio.h>
#include <math.h>

#define MPU6050_ADDR            0x68             /**< Endereço padrão do MPU6050 */
const gpio_num_t SDA_PIN = 21;
const gpio_num_t SCL_PIN = 22;

/**
 * @brief Lê os dados do sensor IMU e os armazena na estrutura IMUData.
 *
 * @param data Ponteiro para a estrutura IMUData onde os dados serão armazenados.
 * @return esp_err_t ESP_OK em caso de sucesso, ou ESP_FAIL em caso de falha.
 */
esp_err_t imu_read_data(IMUData *data) {
    esp_err_t ret;
    ret = imu_init(MPU6050_ADDR, SDA_PIN, SCL_PIN);
    if (ret != ESP_OK) {
        return ESP_FAIL;
    }

    AccelerationData accel_data;
    ret = imu_get_acceleration_data(&accel_data);
    if (ret != ESP_OK) {
        return ESP_FAIL;
    }

    GyroscopeData gyro_data;
    ret = imu_get_gyroscope_data(&gyro_data);
    if (ret != ESP_OK) {
        return ESP_FAIL;
    }

    data->gyro = gyro_data;
    data->accel = accel_data;

    ret = imu_deinit();
    if (ret != ESP_OK) {
        return ESP_FAIL;
    }

    return ESP_OK;
}

/**
 * @brief Calcula o Quaternion a partir dos dados do IMU.
 *
 * @param data Ponteiro para a estrutura IMUData contendo os dados do IMU.
 * @param quaternion Ponteiro para a estrutura Quaternion onde o resultado será armazenado.
 * @return esp_err_t ESP_OK em caso de sucesso, ou ESP_FAIL em caso de falha.
 */
esp_err_t imu_calculate_quaternion(const IMUData *data, Quaternion *quaternion) {
    if (data == NULL || quaternion == NULL) {
        return ESP_FAIL;
    }

    // Normaliza o vetor de aceleração
    float accel_magnitude = sqrtf(data->accel.x * data->accel.x +
                                  data->accel.y * data->accel.y +
                                  data->accel.z * data->accel.z);
    if (accel_magnitude == 0.0f) {
        return ESP_FAIL; // Verifica se a magnitude é zero
    }

    float ax = data->accel.x / accel_magnitude;
    float ay = data->accel.y / accel_magnitude;
    float az = data->accel.z / accel_magnitude;

    // Converte giroscópio de rad/s para graus/s
    float gx_deg = data->gyro.x * (180.0 / M_PI);
    float gy_deg = data->gyro.y * (180.0 / M_PI);
    float gz_deg = data->gyro.z * (180.0 / M_PI);

    // Calcula o Quaternion usando um modelo simplificado
    float pitch = atan2f(-ax, sqrtf(ay * ay + az * az)); // Cálculo do pitch
    float roll = atan2f(ay, az); // Cálculo do roll
    float yaw = atan2f(gz_deg, sqrtf(gx_deg * gx_deg + gy_deg * gy_deg)); // Cálculo do yaw

    float cy = cosf(yaw * 0.5f);
    float sy = sinf(yaw * 0.5f);
    float cr = cosf(roll * 0.5f);
    float sr = sinf(roll * 0.5f);
    float cp = cosf(pitch * 0.5f);
    float sp = sinf(pitch * 0.5f);

    quaternion->w = cr * cp * cy + sr * sp * sy;
    quaternion->x = sr * cp * cy - cr * sp * sy;
    quaternion->y = cr * sp * cy + sr * cp * sy;
    quaternion->z = cr * cp * sy - sr * sp * cy;

    return ESP_OK;
}

/**
 * @brief Calcula os ângulos de Euler a partir de um Quaternion.
 *
 * @param quaternion Ponteiro para a estrutura Quaternion contendo o Quaternion.
 * @param euler Ponteiro para a estrutura EulerAngle onde os ângulos de Euler serão armazenados.
 * @return esp_err_t ESP_OK em caso de sucesso, ou ESP_FAIL em caso de falha.
 */
esp_err_t imu_calculate_euler_angles(const Quaternion *quaternion, EulerAngle *euler) {
    if (quaternion == NULL || euler == NULL) {
        return ESP_FAIL;
    }

    // Extrai os componentes do Quaternion
    float w = quaternion->w;
    float x = quaternion->x;
    float y = quaternion->y;
    float z = quaternion->z;

    // Calcula o roll (rotação ao longo do eixo x)
    float sinr_cosp = 2.0f * (w * x + y * z);
    float cosr_cosp = 1.0f - 2.0f * (x * x + y * y);
    euler->roll = atan2f(sinr_cosp, cosr_cosp);

    // Calcula o pitch (rotação ao longo do eixo y)
    float sinp = 2.0f * (w * y - z * x);
    if (fabsf(sinp) >= 1.0f) {
        euler->pitch = copysignf(M_PI / 2.0f, sinp); // Garante que pitch está dentro do intervalo [-90°, 90°]
    } else {
        euler->pitch = asinf(sinp);
    }

    // Calcula o yaw (rotação ao longo do eixo z)
    float siny_cosp = 2.0f * (w * z + x * y);
    float cosy_cosp = 1.0f - 2.0f * (y * y + z * z);
    euler->yaw = atan2f(siny_cosp, cosy_cosp);

    return ESP_OK;
}
