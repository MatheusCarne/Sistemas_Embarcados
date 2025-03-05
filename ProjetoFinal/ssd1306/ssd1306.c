#include "ssd1306.h"
#include <stdio.h>
#include "driver/i2c.h"
#include "esp_log.h"

uint8_t ssd1306_buffer[SSD1306_WIDTH * SSD1306_HEIGHT / 8];

static const char *TAG = "SSD1306";

// Envia um comando ao SSD1306 via i2c
void ssd1306_send_command(uint8_t command) {
    i2c_cmd_handle_t cmd = i2c_cmd_link_create();
    i2c_master_start(cmd);
    i2c_master_write_byte(cmd, (SSD1306_I2C_ADDRESS << 1) | I2C_MASTER_WRITE, true);
    i2c_master_write_byte(cmd, 0x00, true); // Byte de controle para comandos
    i2c_master_write_byte(cmd, command, true);
    i2c_master_stop(cmd);
    i2c_master_cmd_begin(I2C_MASTER_NUM, cmd, 1000 / portTICK_PERIOD_MS);
    i2c_cmd_link_delete(cmd);
}

// Envia dados para o SSD1306 via i2c
void ssd1306_send_data(uint8_t data) {
    i2c_cmd_handle_t cmd = i2c_cmd_link_create();
    i2c_master_start(cmd);
    i2c_master_write_byte(cmd, (SSD1306_I2C_ADDRESS << 1) | I2C_MASTER_WRITE, true);
    i2c_master_write_byte(cmd, 0x40, true); // Byte de controle para dados
    i2c_master_write_byte(cmd, data, true);
    i2c_master_stop(cmd);
    i2c_master_cmd_begin(I2C_MASTER_NUM, cmd, 1000 / portTICK_PERIOD_MS);
    i2c_cmd_link_delete(cmd);
}

// Inicializa o SSD1306
void ssd1306_init() {
    // Comandos de inicialização do SSD1306
    ssd1306_send_command(0xAE); // Desliga o display
    ssd1306_send_command(0xD5);
    ssd1306_send_command(0x80);
    ssd1306_send_command(0xA8);
    ssd1306_send_command(0x3F);
    ssd1306_send_command(0xD3);
    ssd1306_send_command(0x00);
    ssd1306_send_command(0x40);
    ssd1306_send_command(0x8D);
    ssd1306_send_command(0x14);
    ssd1306_send_command(0x20);
    ssd1306_send_command(0x00);
    ssd1306_send_command(0xA1);
    ssd1306_send_command(0xC8);
    ssd1306_send_command(0xDA);
    ssd1306_send_command(0x12);
    ssd1306_send_command(0x81);
    ssd1306_send_command(0xCF);
    ssd1306_send_command(0xD9);
    ssd1306_send_command(0xF1);
    ssd1306_send_command(0xDB);
    ssd1306_send_command(0x40);
    ssd1306_send_command(0xA4);
    ssd1306_send_command(0xA6);
    ssd1306_send_command(0xAF); // Liga o display
    vTaskDelay(pdMS_TO_TICKS(100));
}

// Limpa o display
void ssd1306_clear() {
    for (uint8_t page = 0; page < 8; page++) {
        ssd1306_send_command(0xB0 + page);
        ssd1306_send_command(0x00);
        ssd1306_send_command(0x10);
        for (uint8_t col = 0; col < 128; col++) {
            ssd1306_send_data(0x00); // Envia um byte de zeros para limpar
        }
    }
}

void ssd1306_clear_line(uint8_t line) {
    ssd1306_send_command(0xB0 + line); // Define a página (linha)
    ssd1306_send_command(0x00); // Define o endereço de coluna baixo
    ssd1306_send_command(0x10); // Define o endereço de coluna alto

    for (uint8_t col = 0; col < 128; col++) {
        ssd1306_send_data(0x00); // Limpa a linha enviando bytes de zero
    }
}

// Fonte básica 5x7 ASCII (apenas números e letras para exemplo)
const uint8_t font5x7[36][5] = {
    // Números 0-9
    {0x7E, 0x81, 0xA5, 0x81, 0x7E}, // '0'
    {0x00, 0x82, 0xFF, 0x80, 0x00}, // '1'
    {0xC2, 0xA1, 0x91, 0x89, 0x86}, // '2'
    {0x42, 0x81, 0x89, 0x89, 0x76}, // '3'
    {0x30, 0x28, 0x24, 0xFF, 0x20}, // '4'
    {0x4F, 0x89, 0x89, 0x89, 0x71}, // '5'
    {0x7E, 0x89, 0x89, 0x89, 0x72}, // '6'
    {0x01, 0xE1, 0x19, 0x05, 0x03}, // '7'
    {0x76, 0x89, 0x89, 0x89, 0x76}, // '8'
    {0x46, 0x89, 0x89, 0x89, 0x7E}, // '9'
    
    // Letras minúsculas a-z
    {0x20, 0x54, 0x54, 0x54, 0x78}, // 'a'
    {0x7F, 0x48, 0x44, 0x44, 0x38}, // 'b'
    {0x38, 0x44, 0x44, 0x44, 0x20}, // 'c'
    {0x38, 0x44, 0x44, 0x48, 0x7F}, // 'd'
    {0x38, 0x54, 0x54, 0x54, 0x18}, // 'e'
    {0x08, 0x7E, 0x09, 0x01, 0x02}, // 'f'
    {0x0C, 0x52, 0x52, 0x52, 0x3E}, // 'g'
    {0x7F, 0x08, 0x04, 0x04, 0x78}, // 'h'
    {0x00, 0x44, 0x7D, 0x40, 0x00}, // 'i'
    {0x20, 0x40, 0x44, 0x3D, 0x00}, // 'j'
    {0x7F, 0x10, 0x28, 0x44, 0x00}, // 'k'
    {0x00, 0x41, 0x7F, 0x40, 0x00}, // 'l'
    {0x7C, 0x04, 0x18, 0x04, 0x78}, // 'm'
    {0x7C, 0x08, 0x04, 0x04, 0x78}, // 'n'
    {0x38, 0x44, 0x44, 0x44, 0x38}, // 'o'
    {0x7C, 0x14, 0x14, 0x14, 0x08}, // 'p'
    {0x08, 0x14, 0x14, 0x18, 0x7C}, // 'q'
    {0x7C, 0x08, 0x04, 0x04, 0x08}, // 'r'
    {0x48, 0x54, 0x54, 0x54, 0x20}, // 's'
    {0x04, 0x3F, 0x44, 0x40, 0x20}, // 't'
    {0x3C, 0x40, 0x40, 0x20, 0x7C}, // 'u'
    {0x1C, 0x20, 0x40, 0x20, 0x1C}, // 'v'
    {0x3C, 0x40, 0x30, 0x40, 0x3C}, // 'w'
    {0x44, 0x28, 0x10, 0x28, 0x44}, // 'x'
    {0x0C, 0x50, 0x50, 0x50, 0x3C}, // 'y'
    {0x44, 0x64, 0x54, 0x4C, 0x44}, // 'z'
};


void ssd1306_draw_char(char c, uint8_t x, uint8_t y) {
    uint8_t index;

    if (c >= '0' && c <= '9') {
        index = c - '0'; // Números começam no índice 0
    } else if (c >= 'a' && c <= 'z') {
        index = c - 'a' + 10; // Letras começam no índice 10
    } else {
        return; // Caractere não suportado
    }

    ssd1306_send_command(0xB0 + y); // Define página
    ssd1306_send_command(0x00 + (x & 0x0F)); // Coluna baixa
    ssd1306_send_command(0x10 + ((x >> 4) & 0x0F)); // Coluna alta

    for (uint8_t i = 0; i < 5; i++) {
        ssd1306_send_data(font5x7[index][i]); // Envia os bytes do caractere
    }

    ssd1306_send_data(0x00); // Espaço entre caracteres
}


void ssd1306_draw_string(const char *str, uint8_t x, uint8_t y) {
    while (*str) {
        if (x + 6 > SSD1306_WIDTH) { // Se a posição x ultrapassar a largura da tela
            x = 0;                   // Volta para o início da linha
            y++;                      // Vai para a próxima linha

            if (y >= 8) {             // Se ultrapassar o limite de páginas (linhas)
                return;               // Para de escrever para evitar erros
            }
        }

        ssd1306_draw_char(*str, x, y); // Desenha o caractere na posição atual
        x += 6;                         // Move para a próxima posição
        str++;                           // Passa para o próximo caractere
    }
}

// Declaração da função `ssd1306_draw_pixel` no início
void ssd1306_draw_pixel(int x, int y, int color);

void ssd1306_draw_bitmap(int x, int y, const uint8_t *bitmap, int width, int height, int color) {
    for (int i = 0; i < height; i++) {
        for (int j = 0; j < width; j++) {
            if (bitmap[i * (width / 8) + j / 8] & (1 << (7 - (j % 8)))) {
                ssd1306_draw_pixel(x + j, y + i, color);
            }
        }
    }
}

void ssd1306_draw_pixel(int x, int y, int color) {
    if (x >= 0 && x < SSD1306_WIDTH && y >= 0 && y < SSD1306_HEIGHT) {
        if (color) {
            // Ativa o pixel
            ssd1306_buffer[y * SSD1306_WIDTH + x] = 1;
        } else {
            // Desativa o pixel
            ssd1306_buffer[y * SSD1306_WIDTH + x] = 0;
        }
    }
}

void ssd1306_update_screen() {
    for (uint8_t i = 0; i < 8; i++) {
        ssd1306_send_command(0xB0 + i); // Seleciona a página (0xB0 a 0xB7 para páginas 0 a 7)
        ssd1306_send_command(0x00);      // Define a coluna inicial (coluna 0)
        ssd1306_send_command(0x10);      // Define a coluna alta (para endereços acima de 128)
        
        for (uint8_t j = 0; j < SSD1306_WIDTH; j++) {
            // Envia os dados dos pixels da linha para o display
            ssd1306_send_data(ssd1306_buffer[i * SSD1306_WIDTH + j]);
        }
    }
}
