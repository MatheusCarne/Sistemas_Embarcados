#ifndef SSD1306_H
#define SSD1306_H

#include <stdio.h>
#include "driver/i2c.h"
#include "esp_log.h"

// Definições do display SSD1306
#define SSD1306_I2C_ADDRESS 0x3C  // Endereço I2C do display
#define SSD1306_WIDTH 128         // Largura do display em pixels
#define SSD1306_HEIGHT 64         // Altura do display em pixels

// Buffer de exibição
extern uint8_t ssd1306_buffer[SSD1306_WIDTH * SSD1306_HEIGHT / 8];

// Inicializa o barramento I2C
void i2c_master_init_ssd();

// Envia um comando para o SSD1306
void ssd1306_send_command(uint8_t command);

// Envia dados para o SSD1306
void ssd1306_send_data(uint8_t data);

// Inicializa o SSD1306
void ssd1306_init();

// Limpa o display
void ssd1306_clear();

// Limpa linha
void ssd1306_clear_line(uint8_t line);

// Desenha um caractere no display
void ssd1306_draw_char(char c, uint8_t x, uint8_t y);

// Desenha uma string no display
void ssd1306_draw_string(const char *str, uint8_t x, uint8_t y);

// Atualiza a tela com o buffer atual
void ssd1306_update_screen();

#endif // SSD1306_H
