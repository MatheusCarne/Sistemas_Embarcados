#ifndef SSR_H
#define SSR_H

#include "driver/gpio.h"
#include "driver/ledc.h"

typedef enum {
    SSR_MODE_ON_OFF,
    SSR_MODE_PWM
} ssr_mode_t;

typedef struct {
    gpio_num_t gpio;     // Pino do SSR
    ssr_mode_t mode;     // Modo de operação (ON-OFF ou PWM)
    uint8_t pwm_channel; // Canal PWM (usado no modo PWM)
    uint32_t pwm_freq;   // Frequência do PWM (usado no modo PWM)
} ssr_t;

void ssr_init(ssr_t *ssr);
void ssr_set_state(ssr_t *ssr, uint8_t state);
void ssr_set_duty(ssr_t *ssr, uint8_t duty);

#endif // SSR_H
