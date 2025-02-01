#include "imu_tools.h"
#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

// Define enumerações para ações
typedef enum {
    ACTION_READ_IMU_DATA,
    ACTION_PRINT_IMU_DATA,
    ACTION_CALCULATE_QUATERNION,
    ACTION_PRINT_QUATERNION,
    ACTION_CALCULATE_EULER_ANGLES,
    ACTION_PRINT_EULER_ANGLES,
    ACTION_NONE // Caso padrão se necessário 
} Action;

/**
 * @brief Função principal do firmware.
 *
 * Lê os dados do IMU, calcula o Quaternion e os ângulos de Euler,
 * e imprime esses dados na saída padrão em um loop infinito.
 */
void app_main() {
    IMUData data;
    Quaternion quaternion;
    EulerAngle eulerAngle;

    // Variável de exemplo para ação, isso poderia ser alterado dinamicamente com base na entrada do usuário ou outra lógica
    Action currentAction = ACTION_READ_IMU_DATA; // Comece lendo os dados do IMU

    while (true) {
        // Verifica se o sensor está conectado ANTES de qualquer ação
        if (imu_read_data(&data) != ESP_OK) {
            printf("\n[ERRO] MPU6050 desconectado! Verifique as conexões e reinicie o sistema.\n");
            vTaskDelay(1000 / portTICK_PERIOD_MS);
            continue; // Aguarda e tenta novamente na próxima iteração
        }

        switch (currentAction) {
            case ACTION_READ_IMU_DATA:
                // Já verificamos a conexão acima, então apenas lê os dados
                break;

            case ACTION_PRINT_IMU_DATA:
                printf("=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=\n");
                printf("Aceleração: X=%.2f, Y=%.2f, Z=%.2f g\n", data.accel.x, data.accel.y, data.accel.z);
                printf("Giroscopio: X=%.2f, Y=%.2f, Z=%.2f °/s\n", data.gyro.x, data.gyro.y, data.gyro.z);
                printf("=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=\n");
                break;

            case ACTION_CALCULATE_QUATERNION:
                if (imu_calculate_quaternion(&data, &quaternion) != ESP_OK) {
                    printf("\n[ERRO] Falha ao calcular quaternion! Sensor pode estar desconectado.\n");
                    continue;
                }
                break;

            case ACTION_PRINT_QUATERNION:
                printf("=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=\n");
                printf("Quaternion: W=%.2f, X=%.2f, Y=%.2f, Z=%.2f\n", quaternion.w, quaternion.x, quaternion.y, quaternion.z);
                printf("=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=\n");
                break;

            case ACTION_CALCULATE_EULER_ANGLES:
                if (imu_calculate_euler_angles(&quaternion, &eulerAngle) != ESP_OK) {
                    printf("\n[ERRO] Falha ao calcular ângulos de Euler! Sensor pode estar desconectado.\n");
                    continue;
                }
                break;

            case ACTION_PRINT_EULER_ANGLES:
                printf("=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=\n");
                printf("Euler Angle: Roll=%.2f, Pitch=%.2f, Yaw=%.2f rad\n", eulerAngle.roll, eulerAngle.pitch, eulerAngle.yaw);
                printf("=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=\n");
                break;

            case ACTION_NONE:
            default:
                break;
        }

        vTaskDelay(1000 / portTICK_PERIOD_MS);
        currentAction = (currentAction + 1) % 7; // Ciclar através das ações
    }
}
