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

## Desenvolvimento de Bibliotecas
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
