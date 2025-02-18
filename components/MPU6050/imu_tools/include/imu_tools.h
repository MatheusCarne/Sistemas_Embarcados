#ifndef IMU_TOOLS_H
#define IMU_TOOLS_H

#include "sensor_imu.h"

/**
 * @brief Define a constante PI
 */
#define PI 3.14159265358979323846

/**
 * @brief Macro para converter graus para radianos.
 *
 * @param deg Valor em graus a ser convertido.
 * @return float Valor correspondente em radianos.
 */
#define DEG_TO_RAD(deg) ((deg) * (PI / 180.0))

/**
 * @brief Estrutura que armazena os dados de aceleração e giroscópio do IMU.
 */
typedef struct {
    GyroscopeData gyro; /**< Dados do giroscópio. */
    AccelerationData accel; /**< Dados de aceleração. */
} IMUData;

/**
 * @brief Estrutura que representa um Quaternion.
 */
typedef struct {
    float w; /**< Parte real do Quaternion. */
    float x; /**< Parte imaginária no eixo X. */
    float y; /**< Parte imaginária no eixo Y. */
    float z; /**< Parte imaginária no eixo Z. */
} Quaternion;

/**
 * @brief Estrutura que representa os ângulos de Euler.
 */
typedef struct {
    float roll;   /**< Rotação ao longo do eixo X em radianos. */
    float pitch;  /**< Rotação ao longo do eixo Y em radianos. */
    float yaw;    /**< Rotação ao longo do eixo Z em radianos. */
} EulerAngle;

esp_err_t imu_read_data(IMUData *data);
esp_err_t imu_calculate_quaternion(const IMUData *data, Quaternion *quaternion);
esp_err_t imu_calculate_euler_angles(const Quaternion *quaternion, EulerAngle *euler); // Usado na ordem ZYX

#endif
