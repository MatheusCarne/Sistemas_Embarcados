#include "servo_tools.h"  // Inclui a biblioteca com funções auxiliares para controle do servo
#include "servo_hw.h"      // Inclui a biblioteca de hardware do servo (PWM, configuração do LEDC)

/**
 * @brief Inicializa o servo motor, configurando o PWM.
 * 
 * @param config Ponteiro para a estrutura de configuração do servo
 * @return esp_err_t ESP_OK se bem-sucedido, ESP_FAIL caso contrário
 */
esp_err_t servo_init(ServoConfig *config) {
  // Verifica se a configuração fornecida é válida
  if (!config) {
    return ESP_FAIL;
  }

  // Inicializa o hardware do servo (configura PWM e pino de controle)
  esp_err_t ret = hw_servo_init(config);

  return ret;  // Retorna o status da inicialização
}

/**
 * @brief Define um ângulo específico para o servo motor.
 * 
 * @param config Ponteiro para a estrutura de configuração do servo
 * @param angle Ângulo desejado (entre 0 e 180 graus)
 * @return esp_err_t ESP_OK se bem-sucedido, ESP_FAIL se houver erro
 */
esp_err_t servo_set_angle(ServoConfig *config, int angle) {
  // Verifica se a configuração é válida e se o ângulo está dentro do intervalo permitido (0° a 180°)
  if (!config || angle < 0 || angle > 181) {
    return ESP_FAIL;
  }

  // Converte o ângulo para um valor de largura de pulso (em microssegundos)
  uint32_t pulse_width_us = SERVO_MIN_PULSE_WIDTH_US + 
                            (angle * (SERVO_MAX_PULSE_WIDTH_US - SERVO_MIN_PULSE_WIDTH_US) / 180);
  
  // Define o pulso PWM para o ângulo desejado
  esp_err_t ret = hw_servo_set_pulse_width(config, pulse_width_us);
  
  // Se a operação for bem-sucedida, atualiza o ângulo atual na estrutura de configuração
  if (ret == ESP_OK) {
    config->current_angle = angle;
  }

  return ret;  // Retorna o status da operação
}

/**
 * @brief Obtém o ângulo atual do servo.
 * 
 * @param config Ponteiro para a estrutura de configuração do servo
 * @param angle Ponteiro para armazenar o ângulo atual
 * @return esp_err_t ESP_OK se bem-sucedido, ESP_FAIL caso contrário
 */
esp_err_t servo_get_angle(const ServoConfig *config, int *angle) {
  // Verifica se os ponteiros fornecidos são válidos
  if (!config || !angle) {
    return ESP_FAIL;
  }

  // Retorna o ângulo armazenado na estrutura de configuração
  *angle = config->current_angle;
  return ESP_OK;
}

/**
 * @brief Desativa o servo motor, desligando o PWM.
 * 
 * @param config Ponteiro para a estrutura de configuração do servo
 * @return esp_err_t ESP_OK se bem-sucedido, ESP_FAIL caso contrário
 */
esp_err_t servo_deinit(ServoConfig *config) {
  // Verifica se a configuração é válida
  if (!config) {
    return ESP_FAIL;
  }

  // Desativa o hardware do servo (para o PWM e libera os recursos)
  esp_err_t ret = hw_servo_deinit(config);

  return ret;  // Retorna o status da desativação
}
