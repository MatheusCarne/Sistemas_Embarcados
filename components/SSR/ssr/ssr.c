#include "ssr.h"
#include "esp_log.h"

#define TAG "SSR_CONTROL"

void ssr_init(ssr_t *ssr) {
    if (ssr->mode == SSR_MODE_ON_OFF) {
        // Configura o GPIO como saída para controle ON-OFF
        gpio_config_t io_conf = {
            .pin_bit_mask = (1ULL << ssr->gpio),
            .mode = GPIO_MODE_OUTPUT,
            .pull_up_en = GPIO_PULLUP_DISABLE,
            .pull_down_en = GPIO_PULLDOWN_DISABLE,
            .intr_type = GPIO_INTR_DISABLE
        };
        gpio_config(&io_conf);
        ESP_LOGI(TAG, "SSR inicializado no modo ON-OFF");
    } else if (ssr->mode == SSR_MODE_PWM) {
        // Configuração do PWM usando LEDC
        ledc_timer_config_t ledc_timer = {
            .speed_mode = LEDC_LOW_SPEED_MODE,
            .timer_num = LEDC_TIMER_0,
            .duty_resolution = LEDC_TIMER_10_BIT,
            .freq_hz = ssr->pwm_freq,
            .clk_cfg = LEDC_AUTO_CLK
        };
        ledc_timer_config(&ledc_timer);

        ledc_channel_config_t ledc_channel = {
            .gpio_num = ssr->gpio,
            .speed_mode = LEDC_LOW_SPEED_MODE,
            .channel = ssr->pwm_channel,
            .timer_sel = LEDC_TIMER_0,
            .duty = 0,
            .hpoint = 0
        };
        ledc_channel_config(&ledc_channel);
        
        ESP_LOGI(TAG, "SSR inicializado no modo PWM, freq: %lu Hz", (unsigned long) ssr->pwm_freq);
    }
}

void ssr_set_state(ssr_t *ssr, uint8_t state) {
    if (ssr->mode == SSR_MODE_ON_OFF) {
        gpio_set_level(ssr->gpio, state);
        ESP_LOGI(TAG, "SSR %s", state ? "LIGADO" : "DESLIGADO");
    }
}

void ssr_set_duty(ssr_t *ssr, uint8_t duty) {
    if (ssr->mode == SSR_MODE_PWM) {
        uint32_t duty_scaled = (duty * 1023) / 100; // Converte para 10 bits (0-1023)
        ledc_set_duty(LEDC_LOW_SPEED_MODE, ssr->pwm_channel, duty_scaled);
        ledc_update_duty(LEDC_LOW_SPEED_MODE, ssr->pwm_channel);
        ESP_LOGI(TAG, "SSR PWM Duty: %d%%", duty);
    }
}
