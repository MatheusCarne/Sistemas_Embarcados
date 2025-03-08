#include "sensor_imu.h"
#include "ssd1306.h"
#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include <math.h>

#define STEP_LENGTH_METERS 0.78  // Comprimento médio da passada
#define THRESHOLD 0.2f           // Limite para detecção de passos no eixo Z
#define DISPLAY_X 10
#define DISPLAY_Y 2

#define MIN_STEP_INTERVAL 500    // Intervalo mínimo de tempo entre passos (em ms)

void app_main() {
    AccelerationData accelData;

    // Inicializa o MPU6050
    esp_err_t ret = imu_init(MPU6050_ADDR, 15, 16);
    if (ret != ESP_OK) {
        printf("Failed to initialize IMU\n");
        return;
    }

    // Inicializa o display SSD1306
    ssd1306_init();
    ssd1306_clear();

    float previous_z = 0.0f;
    int step_count = 0;
    int step_detected = 0;
    int last_step_time = 0;  // Variável para armazenar o último tempo de passo

    char buffer[30];

    // Exibe título inicial no display
    ssd1306_draw_string("passos metros:", DISPLAY_X, 1);

    while (1) {
        ret = imu_get_acceleration_data(&accelData);
        if (ret == ESP_OK) {
            // Verifica se a condição para detectar um passo foi atendida
            if (accelData.z > THRESHOLD && previous_z <= THRESHOLD && !step_detected) {
                step_count++;
                step_detected = 1;
                printf("Passo detectado! Contagem: %d\n", step_count);

                // Atualiza o tempo do último passo
                last_step_time = xTaskGetTickCount();

                
                // Calcula a distância percorrida
                float distance = step_count * STEP_LENGTH_METERS;

                // Apaga área antes de escrever para evitar sobreposição
                ssd1306_clear_line(2);

                // Exibe no display
                snprintf(buffer, sizeof(buffer), "%d /   /%.2fm", step_count, distance);
                printf("%.2f distance\n", distance);
                ssd1306_draw_string(buffer, DISPLAY_X, DISPLAY_Y);
            }

            if (accelData.z <= THRESHOLD) {
                step_detected = 0;
            }

            previous_z = accelData.z;
        } else {
            printf("Erro ao ler dados do acelerômetro.\n");
        }

        // Garantir que o tempo entre os passos seja mínimo
        if ((xTaskGetTickCount() - last_step_time) < pdMS_TO_TICKS(MIN_STEP_INTERVAL)) {
            vTaskDelay(10 / portTICK_PERIOD_MS);  // Adiciona um pequeno delay se o intervalo entre os passos for muito curto
        }

        vTaskDelay(100 / portTICK_PERIOD_MS);
    }
}
