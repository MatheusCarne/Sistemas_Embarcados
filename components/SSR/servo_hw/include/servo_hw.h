
#ifndef SERVO_HW_H
#define SERVO_HW_H

#include "esp_err.h"
#include "esp_log.h"
#include "driver/ledc.h"
#include "driver/gpio.h"
#include "servo_tools.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

// Inicializa o GPIO para a geração do sinal PWM usado no controle do servo
esp_err_t hw_servo_init(ServoConfig *config); 
// Define a largura de pulso (em microsegundos) para controlar o ângulo do servomotor.
esp_err_t hw_servo_set_pulse_width(ServoConfig *config, uint32_t pulse_width_us);
// Desabilita o PWM no GPIO especificado e libera os recursos utilizados
esp_err_t hw_servo_deinit(ServoConfig *config);

#endif // SERVO_HW_H
