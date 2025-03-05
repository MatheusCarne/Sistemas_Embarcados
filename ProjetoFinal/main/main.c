#include "sensor_imu.h"
#include "ssd1306.h"
#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include <math.h>

void app_main() {
    AccelerationData accelData;

    // Inicializa o MPU6050
    esp_err_t ret = imu_init(MPU6050_ADDR, 21, 22); // SDA = 21, SCL = 22
    if (ret != ESP_OK) {
        printf("Failed to initialize IMU\n");
        return;
    }

    // Inicializa o display SSD1306
    ssd1306_init();
    ssd1306_clear();

    float previous_z = 0.0f;
    float threshold = 0.2f; // Limite para detecção de pico
    int step_count = 0;
    int step_detected = 0; // Flag para evitar contagem múltipla de um único passo

    char buffer[20];

    snprintf(buffer, sizeof(buffer), "passos:"); // Formata o valor de step_count
    ssd1306_draw_string(buffer, 10, 1);
    
    while (1) {
    ret = imu_get_acceleration_data(&accelData);
    if (ret == ESP_OK) {
        // printf("Aceleração - X: %.2f g, Y: %.2f g, Z: %.2f g\n", accelData.x, accelData.y, accelData.z);

        // Detecção de passos com base no eixo Z
        if (accelData.z > threshold && previous_z <= threshold && !step_detected) {
            step_count++;
            step_detected = 1; // Marca que um passo foi detectado
            printf("Passo detectado! Contagem de passos: %d\n", step_count);

            snprintf(buffer, sizeof(buffer), "%d", step_count);
            ssd1306_draw_string(buffer, 10, 2); 
        }

        // Reset da flag quando o valor de Z cai abaixo do threshold
        if (accelData.z <= threshold) {
            step_detected = 0;
        }

        previous_z = accelData.z;
    } else {
        printf("Erro ao ler os dados de aceleração.\n");
    }

    vTaskDelay(100 / portTICK_PERIOD_MS);  // Atraso de 100ms entre as leituras
}
}
