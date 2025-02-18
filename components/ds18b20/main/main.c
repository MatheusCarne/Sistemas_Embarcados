#include "ds18b20.h"
#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

void app_main() {
    printf("Iniciando leitura do sensor DS18B20...\n");

    // Inicializa o pino: reseta, configura pull-up e seta como entrada inicialmente
    gpio_reset_pin(DS18B20_GPIO);
    gpio_set_pull_mode(DS18B20_GPIO, GPIO_PULLUP_ONLY);
    gpio_set_direction(DS18B20_GPIO, GPIO_MODE_INPUT);

    // Teste isolado do reset: imprima a cada 2 segundos se o sensor é detectado ou não
    while (1) {
        // Teste de reset isolado (descomente para testar somente o reset)
        /*
        if (ds18b20_reset()) {
            printf("DEBUG: Sensor detectado!\n");
        } else {
            printf("DEBUG: Sensor não detectado!\n");
        }
        vTaskDelay(pdMS_TO_TICKS(2000));
        continue;
        */
        // Leitura completa da temperatura
        float temperature = ds18b20_get_temperature();
        if (temperature != -0.06) { // Verifica se a leitura é válida
            printf("Temperatura: %.2f°C\n", temperature);
        } else {
            printf("Erro ao ler temperatura.\n");
        } 
        vTaskDelay(pdMS_TO_TICKS(2000)); // Aguarda 2 segundos antes da próxima leitura
    }
}
