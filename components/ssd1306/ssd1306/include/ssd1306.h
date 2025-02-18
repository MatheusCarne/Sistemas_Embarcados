#ifndef SSD1306_H
#define SSD1306_H

#include <stdio.h>
#include "driver/i2c.h"
#include "esp_log.h"

// Definições do barramento I2C
#define I2C_MASTER_SCL_IO 22       // Pino do SCL
#define I2C_MASTER_SDA_IO 21       // Pino do SDA
#define I2C_MASTER_FREQ_HZ 100000  // Frequência do I2C
#define I2C_MASTER_NUM I2C_NUM_0   // Número do barramento I2C
// Definições do display SSD1306
#define SSD1306_I2C_ADDRESS 0x3C  // Endereço I2C do display

#define SSD1306_WIDTH 128         // Largura do display em pixels
#define SSD1306_HEIGHT 64         // Altura do display em pixels

// Buffer de exibição
extern uint8_t ssd1306_buffer[SSD1306_WIDTH * SSD1306_HEIGHT / 8];

// ============================================
//        Definições do barramento SPI
// ============================================
// Definições do SPI
/*#define SPI_MOSI_IO 17             // Pino MOSI (DIN do display)
#define SPI_SCLK_IO 16             // Pino SCLK (CLK do display)
#define SPI_CS_IO 15               // Pino CS (Chip Select)
#define SPI_DC_IO 14               // Pino DC (Data/Command)
#define SPI_RESET_IO 13            // Pino RESET (opcional)
static spi_device_handle_t spi;    // Handle do dispositivo SPI
*/

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

// Desenha um caractere no display
void ssd1306_draw_char(char c, uint8_t x, uint8_t y);

// Desenha uma string no display
void ssd1306_draw_string(const char *str, uint8_t x, uint8_t y);

// Atualiza a tela com o buffer atual
void ssd1306_update_screen();

#endif // SSD1306_H
