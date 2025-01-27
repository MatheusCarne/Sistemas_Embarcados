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
        switch (currentAction) {
            case ACTION_READ_IMU_DATA:
                // Ler dados do IMU
                imu_read_data(&data);
                break;

            case ACTION_PRINT_IMU_DATA:
                // Print dados do IMU
                printf("=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=\n");
                printf("Aceleração: X=%.2f, Y=%.2f, Z=%.2f g\n", data.accel.x, data.accel.y, data.accel.z);
                printf("Giroscopio: X=%.2f, Y=%.2f, Z=%.2f °/s\n", data.gyro.x, data.gyro.y, data.gyro.z);
                printf("=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=\n");
                break;

            case ACTION_CALCULATE_QUATERNION:
                // Calcular quaternião
                imu_calculate_quaternion(&data, &quaternion);
                break;

            case ACTION_PRINT_QUATERNION:
                // Print dados do Quaternion
                printf("=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=\n");
                printf("Quaternion: W=%.2f, X=%.2f, Y=%.2f, Z=%.2f\n", quaternion.w, quaternion.x, quaternion.y, quaternion.z);
                printf("=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=\n");
                break;

            case ACTION_CALCULATE_EULER_ANGLES:
                // Calcular ângulos de Euler
                imu_calculate_euler_angles(&quaternion, &eulerAngle);
                break;

            case ACTION_PRINT_EULER_ANGLES:
                // Print dados do ângulo de Euler
                printf("=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=\n");
                printf("Euler Angle: Roll=%.2f, Pitch=%.2f, Yaw=%.2f rad\n", eulerAngle.roll, eulerAngle.pitch, eulerAngle.yaw);
                printf("=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=\n");
                break;

            case ACTION_NONE:
            default:
                // Não faça nada ou lide com outras ações
                break;
        }

        vTaskDelay(100 / portTICK_PERIOD_MS);

        // Exemplo de lógica para mudar ação, poderia ser baseado na entrada do usuário ou outras condições
        currentAction = (currentAction + 1) % 7; // Ciclar através das ações
    }
}
