/*motor.c debe tner la logica de encapsulamiento*/

// se recibe de eje.c dos estructuras de configuracion de motor
// la funcion initialize devuelve dos punteros (pero hasta ahora no son necesarios...)

#include <stdio.h>

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/gpio.h"
#include "esp_attr.h"
#include <stdlib.h>
#include <string.h>
#include <sys/cdefs.h>
#include "esp_log.h"
#include "esp_check.h"
#include "motor.h"

#include "bdc_motor.h"

#define BDC_MCPWM_TIMER_RESOLUTION_HZ 10000000                                      // 10MHz, 1 tick = 0.1us
#define BDC_MCPWM_FREQ_HZ 25000                                                     // 25KHz PWM
#define BDC_MCPWM_DUTY_TICK_MAX (BDC_MCPWM_TIMER_RESOLUTION_HZ / BDC_MCPWM_FREQ_HZ) // maximum value we can set for the duty cycle, in ticks

static const char *TAG = "Motor";

bdc_motor_handle_t motor = NULL;

bdc_motor_handle_t Crear_motor (gpio_num_t pin_p, gpio_num_t pin_n)
{
    
    bdc_motor_config_t motor_config = {
        .pwm_freq_hz = BDC_MCPWM_FREQ_HZ,
        .pwma_gpio_num = pin_p,
        .pwmb_gpio_num = pin_n,
    };

    bdc_motor_mcpwm_config_t mcpwm_config = {
        .group_id = 0,
        .resolution_hz = BDC_MCPWM_TIMER_RESOLUTION_HZ,
    };
    ESP_ERROR_CHECK(bdc_motor_new_mcpwm_device(&motor_config, &mcpwm_config, &motor));
    ESP_LOGI(TAG, "Enable motor");
    ESP_ERROR_CHECK(bdc_motor_enable(motor));
    ESP_LOGI(TAG, "Stop motor");
    ESP_ERROR_CHECK(bdc_motor_brake(motor));
    
    return motor;

}