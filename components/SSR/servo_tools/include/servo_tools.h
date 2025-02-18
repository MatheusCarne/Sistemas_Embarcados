#ifndef SERVO_TOOLS_H
#define SERVO_TOOLS_H

#include "esp_err.h"
#include "driver/ledc.h"

// Define constants for PWM
#define SERVO_PWM_FREQUENCY 50 // PWM frequency in Hz
#define SERVO_PWM_RESOLUTION LEDC_TIMER_13_BIT // PWM resolution
#define SERVO_MIN_PULSE_WIDTH_US 500 // Minimum pulse width in microseconds
#define SERVO_MAX_PULSE_WIDTH_US 2500 // Maximum pulse width in microseconds

typedef struct {
  uint8_t gpio_num;          // Porta GPIO onde é usado o PWM
  ledc_mode_t mode;          // Modo do PWM
  ledc_channel_t channel;    // Canal do PWM
  ledc_timer_t timer;        // Temporizador do PWM
  uint32_t frequency;        // Frequencia em HZ
  uint32_t duty_resolution;  // Resolução do Duty
  int current_angle; // Adiciona um campo para armazenar o ângulo atual

} ServoConfig;

typedef int ServoAngle;

esp_err_t servo_init(ServoConfig *config); // Inicializa o servomotor com base na configuração fornecida.
esp_err_t servo_set_angle(ServoConfig *config, int angle); // Define o ângulo do servomotor, movendo o braço acoplado ao rotor para a posição desejada.
esp_err_t servo_get_angle(const ServoConfig *config, int *angle); // Obtém o ângulo atual do braço do servomotor.
esp_err_t servo_deinit(ServoConfig *config);

#endif // SERVO_TOOLS_H