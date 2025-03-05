# Projeto feito em conjunto com: 

<div style="display: inline_block;" align="center">

<table>
  <tr>
    <td align="center">
      <a href="https://github.com/MatheusCarne" target="_blank">
        <img src="https://avatars.githubusercontent.com/u/88046644?v=4" width="100px;" alt="Avatar Matheus"/><br>
        <sub>
          <b>Matheus Carneiro</b>
        </sub>
      </a>
    </td>
    <td align="center">
      <a href="https://github.com/RafaelBerg" target="_blank">
        <img src="https://avatars.githubusercontent.com/u/86807247?v=4" width="100px;" alt="Avatar Rafael"/><br>
        <sub>
          <b>Rafael Berg</b>
        </sub>
      </a>
    </td>
    <td align="center">
      <a href="https://github.com/heitorbrunini" target="_blank">
        <img src="https://avatars.githubusercontent.com/u/68927093?v=4" width="100px;" alt="Avatar Thiago"/><br>
        <sub>
          <b>Heitor Brunini</b>
        </sub>
      </a>
    </td>
  </tr>
</table>

</div>

| Placas suportadas | ESP32 | ESP32-C3 | ESP32-C5 | ESP32-C6 | ESP32-C61 | ESP32-H2 | ESP32-S3 |
| ----------------- | ----- | -------- | -------- | -------- | --------- | -------- | -------- |

# Desenvolvimento de Bibliotecas
Foram desenvolvidas bibliotecas para os seguintes módulos/sensores:

### MPU6050 (I2C)
- **Descrição:** Biblioteca para leitura dos dados do acelerômetro e giroscópio via protocolo I2C.
- **Funcionalidades:**
  - Leitura de aceleração nos eixos X, Y e Z.
  - Leitura de rotação nos eixos X, Y e Z.
  - Cálculo de Quartenions.
  - Cálculo do Ângulo de Euler.

### SSD1306 (I2C)
- **Descrição:** Biblioteca para controle do display OLED SSD1306 via comunicação I2C.
- **Funcionalidades:**
  - Exibição de texto.
  - Desenho de imagens bitmap.

### DS18B20 (1-Wire)
- **Descrição:** Biblioteca para leitura de temperatura utilizando o sensor DS18B20 via comunicação 1-Wire.
- **Funcionalidades:**
  - Leitura de temperatura com resolução configurável.
  - Suporte a múltiplos sensores no mesmo barramento.
  - Conversão automática para graus Celsius.

### SSR (Relé de Estado Sólido)
- **Descrição:** Biblioteca para controle de um Relé de Estado Sólido (SSR) com suporte a acionamento ON-OFF e PWM.
- **Funcionalidades:**
- Modulação por Largura de Pulso (PWM) para controle de potência.

# Descrição projeto de disciplina:
## Contador de Passos com ESP32
## Introdução

Este projeto consiste em um contador de passos (pedômetro) desenvolvido para a plataforma ESP32. O objetivo é monitorar e exibir o número de passos dados por um usuário, utilizando sensores e recursos do ESP32. O sistema é ideal para aplicações de fitness, saúde ou monitoramento de atividade física.

O projeto pode ser integrado a displays, sensores e outros periféricos para fornecer uma solução completa de monitoramento de passos.

---

## Componentes Utilizados

- **ESP32**: Microcontrolador principal, responsável por processar os dados do sensor e gerenciar a exibição das informações.
- **MPU6050**: Acelerômetro Detecta movimentos do usuário e envia os dados para o ESP32.
- **SSD1306**: Exibe o número de passos em tempo real.

---

## Funcionalidades do Sistema

### 1. Contagem de Passos:
- O algoritmo de detecção de passos processa os dados do sensor para identificar movimentos característicos de caminhada ou corrida.

### 2. Exibição do Número de Passos:
- O número de passos é exibido em um display OLED.

### 3. Reset do Contador:
- O contador de passos pode ser zerado manualmente ou automaticamente após um período de inatividade.

### 4. Integração com Periféricos:
- O projeto pode ser expandido para incluir funcionalidades adicionais, como:
  - Medição de distância percorrida.
  - Cálculo de calorias queimadas.
  - Sincronização com um aplicativo móvel para armazenamento e análise de dados.

---

## Integração das Bibliotecas

- **Biblioteca MPU6050**: Responsável pela leitura dos dados do acelerômetro via I2C.
- **Biblioteca SSD1306**: Permite a comunicação com o display OLED via I2C.

---

## Fluxo de Funcionamento

1. **Inicialização**
   - Configuração do ESP32, inicialização do sensor e do display.

2. **Leitura do Sensor**
   - O sensor de movimento é lido periodicamente para capturar dados de aceleração.

3. **Processamento de Dados**
   - Um algoritmo processa os dados do sensor para identificar padrões de movimento correspondentes a passos.

4. **Atualização do Contador**
   - O número de passos é incrementado sempre que um passo é detectado.

5. **Exibição dos Dados**
   - O número de passos é exibido no display ou enviado para um dispositivo externo.

---
