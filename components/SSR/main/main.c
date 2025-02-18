#include "servo_tools.h"
#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_log.h"

#define GPIO_SERVO 12  // Pino do servo
#define DELAY_MS 1000  // Tempo de espera entre os movimentos

static const char *TAG = "SERVO_CONTROL";

void app_main() {
    ESP_LOGI(TAG, "Iniciando controle do servo...");

    // Configuração do Servo
    ServoConfig servo = {
        .gpio_num = GPIO_SERVO,
        .mode = LEDC_HIGH_SPEED_MODE,
        .channel = LEDC_CHANNEL_0,
        .timer = LEDC_TIMER_0,
        .frequency = SERVO_PWM_FREQUENCY,
        .duty_resolution = SERVO_PWM_RESOLUTION,
        .current_angle = 0
    };

    // Inicializa o servo
    if (servo_init(&servo) != ESP_OK) {
        ESP_LOGE(TAG, "Falha ao inicializar o Servo");
        return;
    }

    ESP_LOGI(TAG, "Servo inicializado com sucesso!");

    // Loop para movimentar o servo continuamente
    while (1) {
        ESP_LOGI(TAG, "Movendo o servo...");

        // Move para 0 graus
        servo_set_angle(&servo, 0);
        ESP_LOGI(TAG, "Servo em 0°");
        vTaskDelay(pdMS_TO_TICKS(DELAY_MS));

        // Move para 90 graus
        servo_set_angle(&servo, 90);
        ESP_LOGI(TAG, "Servo em 90°");
        vTaskDelay(pdMS_TO_TICKS(DELAY_MS));

        // Move para 180 graus
        servo_set_angle(&servo, 180);
        ESP_LOGI(TAG, "Servo em 180°");
        vTaskDelay(pdMS_TO_TICKS(DELAY_MS));

        // Volta para 0 graus
        servo_set_angle(&servo, 0);
        ESP_LOGI(TAG, "Retornando o servo para 0°");
        vTaskDelay(pdMS_TO_TICKS(DELAY_MS));
    }
}
