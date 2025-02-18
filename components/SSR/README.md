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

### Descrição do projeto

Neste repositório foram desenvolvidas duas bibliotecas para utilizar do SSR.

Na biblioteca **servo_hw** Configura o timer e o canal PWM e demais configurações.

```
hw_servo_init( ): Configura o timer e o canal PWM e Testa o servo movendo-o de 0° a 180° e de volta para 0°.

hw_servo_set_pulse_width( ): Converte a largura de pulso para um valor de duty cycle e Define o duty cycle no canal do PWM.

hw_servo_deinit( ): Para o PWM e pausa o timer, desligando o controle do servo.

```

Na **servo_tools**   

```
servo_init( ): Chama hw_servo_init( ) para configurar o PWM e preparar o controle do servo.

servo_set_angle( ): Verifica se o ponteiro config é válido e se o ângulo está entre 0° e 180°, Converte o ângulo em largura de pulso, etc.

servo_get_angle( ): Retorna o ângulo atual armazenado em config->current_angle.

servo_deinit( ): Chama hw_servo_deinit( ) para desligar o PWM e liberar os recursos.
```

