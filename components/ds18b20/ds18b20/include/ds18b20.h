#ifndef DS18B20_H
#define DS18B20_H

#include <stdint.h>

// Definição do pino do sensor DS18B20
#define DS18B20_GPIO ((gpio_num_t)5)

// Função para reset do sensor DS18B20
void ds18b20_send_reset();

// Funções para leitura de dados
uint8_t ds18b20_read_bit();
uint8_t ds18b20_read_byte();

// Funções para escrita de dados
void ds18b20_write_bit(uint8_t bit);
void ds18b20_write_byte(uint8_t data);

// Função para obter a temperatura
float ds18b20_get_temperature();

#endif // DS18B20_H