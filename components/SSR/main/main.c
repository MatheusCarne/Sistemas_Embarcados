#include "ssr.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"


void app_main(void) {
    ssr_t ssr = {
        .gpio = GPIO_NUM_9,
        .mode = SSR_MODE_ON_OFF, // SSR_MODE_ON_OFF ou SSR_MODE_PWM
        .pwm_channel = LEDC_CHANNEL_0,
        .pwm_freq = 1000 // Frequência PWM de 1 kHz
    };

    ssr_init(&ssr);

    while (1) {
        //Modo ON/OFF
        ssr.mode = SSR_MODE_ON_OFF;
        ssr_init(&ssr);

        // Testando ON/OFF
        ssr_set_state(&ssr, 1);
        vTaskDelay(pdMS_TO_TICKS(1000));

        ssr_set_state(&ssr, 0);
        vTaskDelay(pdMS_TO_TICKS(1000));

        ssr_set_state(&ssr, 1);
        vTaskDelay(pdMS_TO_TICKS(1000));

        //Modo PWM
        ssr.mode = SSR_MODE_PWM;
        ssr_init(&ssr);


        // Testando PWM
        for (int i = 100; i >= 0; i -= 10) {
            ssr_set_duty(&ssr, i);
            vTaskDelay(pdMS_TO_TICKS(250));
        }

        for (int i = 0; i <= 100; i += 10) {
            ssr_set_duty(&ssr, i);
            vTaskDelay(pdMS_TO_TICKS(250));
        }

    }
}
