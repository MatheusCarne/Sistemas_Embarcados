#include "servo_hw.h"  // Inclui o cabeçalho com as definições necessárias para controlar o servo

/*
 * @brief Inicializa o servo configurando o timer e o canal PWM
 * 
 * @param config Ponteiro para a estrutura de configuração do servo
 * @return esp_err_t ESP_OK se bem-sucedido, ESP_FAIL em caso de erro
 */
esp_err_t hw_servo_init(ServoConfig *config) {
  // Configuração do timer do LEDC (usado para gerar o sinal PWM)
  ledc_timer_config_t ledc_timer = {
    .speed_mode = config->mode,        // Define o modo de operação (alta ou baixa velocidade)
    .timer_num = config->timer,        // Define qual timer será usado
    .duty_resolution = config->duty_resolution, // Resolução do duty cycle (ex: 10 bits)
    .freq_hz = config->frequency,      // Frequência do PWM para o servo
    .clk_cfg = LEDC_AUTO_CLK           // Configuração automática do clock
  };
  
  // Aplica a configuração do timer
  esp_err_t ret = ledc_timer_config(&ledc_timer);
  if (ret != ESP_OK) {
    return ESP_FAIL;  // Retorna falha caso a configuração do timer não seja bem-sucedida
  }

  // Configuração do canal do LEDC (associado ao GPIO do servo)
  ledc_channel_config_t ledc_channel = {
    .gpio_num = config->gpio_num,  // Define o pino GPIO onde o sinal PWM será gerado
    .speed_mode = config->mode,    // Modo de operação (alta ou baixa velocidade)
    .channel = config->channel,    // Define qual canal será usado para gerar o PWM
    .timer_sel = config->timer,    // Associa o canal ao timer configurado anteriormente
    .duty = 0,                     // Inicializa o duty cycle como 0
    .hpoint = 0                    // Define o ponto de início do PWM
  };

  // Aplica a configuração do canal
  ret = ledc_channel_config(&ledc_channel);
  if (ret != ESP_OK) {
    return ESP_FAIL;  // Retorna falha caso a configuração do canal não seja bem-sucedida
  }

  // Teste inicial do servo: move de 0° a 180° e depois volta para 0°
  for (int angle = 0; angle <= 180; angle += 10) {
    hw_servo_set_pulse_width(config, 500 + (angle * 2000 / 180));  // Calcula e define o pulso PWM para o ângulo
    vTaskDelay(pdMS_TO_TICKS(100));  // Aguarda 100ms para o movimento
  }
  for (int angle = 180; angle >= 0; angle -= 10) {
    hw_servo_set_pulse_width(config, 500 + (angle * 2000 / 180));  // Define novamente o pulso PWM para o ângulo
    vTaskDelay(pdMS_TO_TICKS(100));  // Aguarda 100ms para o movimento
  }

  return ESP_OK;  // Retorna sucesso após a configuração completa
}

/*
 * @brief Define a largura do pulso PWM para controlar o ângulo do servo
 * 
 * @param config Ponteiro para a estrutura de configuração do servo
 * @param pulse_width_us Largura do pulso em microssegundos (us)
 * @return esp_err_t ESP_OK se bem-sucedido, ESP_FAIL se o valor estiver fora dos limites
 */
esp_err_t hw_servo_set_pulse_width(ServoConfig *config, uint32_t pulse_width_us) {
  // Verifica se o valor do pulso está dentro dos limites mínimos e máximos permitidos para o servo
  if (pulse_width_us < SERVO_MIN_PULSE_WIDTH_US || pulse_width_us > SERVO_MAX_PULSE_WIDTH_US) {
    return ESP_FAIL;  // Retorna erro caso o valor esteja fora do intervalo permitido
  }

  // Calcula o ciclo de trabalho (duty cycle) baseado na largura do pulso desejada
  uint32_t duty_cycle = (pulse_width_us * (1 << SERVO_PWM_RESOLUTION)) / (1000000 / SERVO_PWM_FREQUENCY);

  // Define o duty cycle no canal configurado
  esp_err_t ret = ledc_set_duty(config->mode, config->channel, duty_cycle);
  if (ret == ESP_OK) {
    ledc_update_duty(config->mode, config->channel);  // Atualiza o duty cycle para aplicar a mudança
  }

  return ret;  // Retorna ESP_OK se bem-sucedido, ou um código de erro caso contrário
}

/*
 * @brief Desativa o servo parando o PWM e pausando o timer
 * 
 * @param config Ponteiro para a estrutura de configuração do servo
 * @return esp_err_t ESP_OK se bem-sucedido, ESP_FAIL em caso de erro
 */
esp_err_t hw_servo_deinit(ServoConfig *config) {
  esp_err_t ret;

  // Para o sinal PWM no canal especificado
  ret = ledc_stop(LEDC_HIGH_SPEED_MODE, config->channel, 0);
  if (ret != ESP_OK) {
    return ESP_FAIL;  // Retorna falha se não conseguir parar o PWM
  }

  // Pausa o timer do PWM para economizar energia
  ret = ledc_timer_pause(LEDC_HIGH_SPEED_MODE, LEDC_TIMER_0);
  if (ret != ESP_OK) {
    return ESP_FAIL;  // Retorna falha se não conseguir pausar o timer
  }

  return ESP_OK;  // Retorna sucesso após desativar o servo
}
