#include "ssd1306.h"
#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

void app_main() {
    i2c_master_init_ssd();
    ssd1306_init();
    ssd1306_clear();

    while (1) {
    // Escreve um texto na tela
    ssd1306_draw_string("esta funcionando", 10, 0);  // Ajuste a posição e tamanho do texto    
    
    // Desenha um retângulo (exemplo de forma geométrica)
    //draw_simple_rectangle();  // Tenta desenhar um retângulo simples
    
    // Desenha um bitmap (exemplo de uma imagem)
    //ssd1306_draw_bitmap(0, 0, logo_bitmap, 128, 64, 1); // Coloca o logo no topo da tela
    vTaskDelay(pdMS_TO_TICKS(2000));
    ssd1306_update_screen(); // Atualiza a tela para exibir o bitmap
    }
}
