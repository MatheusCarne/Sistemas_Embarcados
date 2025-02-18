#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/gpio.h"
#include "rom/ets_sys.h"

#define DS18B20_GPIO ((gpio_num_t)5)

// Função de reset com detecção de presença
// Retorna 1 se o sensor respondeu (presence pulse detectado) ou 0 caso contrário.
uint8_t ds18b20_reset() {
    uint8_t presence;

    // Configura o pino como saída e ativa o pull-up interno
    gpio_set_direction(DS18B20_GPIO, GPIO_MODE_OUTPUT);
    gpio_set_pull_mode(DS18B20_GPIO, GPIO_PULLUP_ONLY);

    // Envia pulso de reset (mantém LOW por 500µs)
    gpio_set_level(DS18B20_GPIO, 0);
    ets_delay_us(500);

    // Libera a linha: seta HIGH e aguarda 80µs para o sensor responder
    gpio_set_level(DS18B20_GPIO, 1);
    ets_delay_us(80);

    // Configura o pino como entrada e mantém o pull-up
    gpio_set_direction(DS18B20_GPIO, GPIO_MODE_INPUT);
    gpio_set_pull_mode(DS18B20_GPIO, GPIO_PULLUP_ONLY);

    // Lê o nível: o sensor deve puxar a linha para 0 (presence pulse)
    presence = gpio_get_level(DS18B20_GPIO);
    ets_delay_us(500);  // Aguarda para completar o slot

    printf("DEBUG: ds18b20_reset - Nível de presença lido: %d\n", presence);
    return (presence == 0) ? 1 : 0;
}

// Função para ler um único bit do sensor
uint8_t ds18b20_read_bit() {
    uint8_t bit;

    gpio_set_direction(DS18B20_GPIO, GPIO_MODE_OUTPUT);
    gpio_set_pull_mode(DS18B20_GPIO, GPIO_PULLUP_ONLY);
    gpio_set_level(DS18B20_GPIO, 0);
    ets_delay_us(2);  // Pulso de 2µs
    gpio_set_direction(DS18B20_GPIO, GPIO_MODE_INPUT);
    gpio_set_pull_mode(DS18B20_GPIO, GPIO_PULLUP_ONLY);
    ets_delay_us(12); // Aguarda 12µs para que o sensor defina o nível
    bit = gpio_get_level(DS18B20_GPIO);
    ets_delay_us(46); // Completa o slot de tempo de 60µs
    return bit;
}

// Função para ler um byte do sensor (LSB primeiro)
uint8_t ds18b20_read_byte() {
    uint8_t byte = 0;
    for (int i = 0; i < 8; i++) {
        byte |= (ds18b20_read_bit() << i);
    }
    return byte;
}

// Função para escrever um único bit no sensor
void ds18b20_write_bit(uint8_t bit) {
    gpio_set_direction(DS18B20_GPIO, GPIO_MODE_OUTPUT);
    gpio_set_pull_mode(DS18B20_GPIO, GPIO_PULLUP_ONLY);
    gpio_set_level(DS18B20_GPIO, 0);
    if (bit) {
        ets_delay_us(2);   // Para escrever 1: LOW por 2µs
        gpio_set_level(DS18B20_GPIO, 1);
        ets_delay_us(60);  // Libera por 60µs
    } else {
        ets_delay_us(60);  // Para escrever 0: LOW por 60µs
        gpio_set_level(DS18B20_GPIO, 1);
        ets_delay_us(2);   // Libera por 2µs
    }
}

// Função para escrever um byte no sensor (LSB primeiro)
void ds18b20_write_byte(uint8_t data) {
    for (int i = 0; i < 8; i++) {
        ds18b20_write_bit(data & (1 << i));
    }
}

// Função para obter a temperatura do sensor
float ds18b20_get_temperature() {
    // Inicia a comunicação e verifica a presença
    if (!ds18b20_reset()) {
        printf("Erro: DS18B20 não respondeu ao reset!\n");
        return -0.06;  // Valor de erro
    }
    // Envia comandos: Skip ROM e Convert T
    ds18b20_write_byte(0xCC); // Skip ROM
    ds18b20_write_byte(0x44); // Inicia a conversão de temperatura

    // Aguarda tempo suficiente para a conversão (aqui usamos 2,5s para garantir)
    vTaskDelay(pdMS_TO_TICKS(3000));  // Espera 1 segundo (1000ms)

    // Inicia nova comunicação para ler o scratchpad
    if (!ds18b20_reset()) {
        printf("Erro: DS18B20 não respondeu ao reset (leitura)!\n");
        return -0.06;
    }
    ds18b20_write_byte(0xCC); // Skip ROM
    ds18b20_write_byte(0xBE); // Comando de leitura do scratchpad

    // Lê os 2 primeiros bytes (temperatura)
    uint8_t lsb = ds18b20_read_byte();
    uint8_t msb = ds18b20_read_byte();

    // Debug: Mostra os valores lidos
    printf("DEBUG: LSB: %02X, MSB: %02X\n", lsb, msb);

    // Se os valores lidos forem 0xFF, pode indicar que o sensor não respondeu
    if (lsb == 0xFF && msb == 0xFF) {
        printf("Erro: Leitura inválida do scratchpad (LSB: 0xFF, MSB: 0xFF)\n");
        return -0.06;
    }

    // Converte os dois bytes em um valor de 16 bits
    int16_t raw_temp = (msb << 8) | lsb;
    // Converte para temperatura (dividindo por 16.0, conforme resolução de 12 bits)
    float temperature = raw_temp / 16.0;

    // Verifica se a temperatura está dentro da faixa válida
    if (temperature < -55.0 || temperature > 125.0) {
        printf("Valor de temperatura fora da faixa válida: %.2f°C\n", temperature);
        return -0.06;
    }

    return temperature;
}