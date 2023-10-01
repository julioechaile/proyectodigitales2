/*motor.c crea y gestiona los motores
crear motor: crea el objeto motor y lo configura, luego devuelve el handle del motor creado
comandos de motor:reciben el handle y el duty y actual sobre las funciones de la API*/
/*motor.c debe tner la logica de encapsulamiento*/

#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/queue.h"
#include "esp_attr.h"
#include <stdlib.h>
#include <string.h>
#include <sys/cdefs.h>
#include "esp_log.h"
#include "esp_check.h"
#include "bdc_motor.h"
#include "motor.h"
#include "config.h"

#define BDC_MCPWM_TIMER_RESOLUTION_HZ 10000000                                      // 10MHz, 1 tick = 0.1us
#define BDC_MCPWM_FREQ_HZ 25000                                                     // 25KHz PWM
#define BDC_MCPWM_DUTY_TICK_MAX (BDC_MCPWM_TIMER_RESOLUTION_HZ / BDC_MCPWM_FREQ_HZ) // maximum value we can set for the duty cycle, in ticks

static const char *TAG = "Motor";

// bdc_motor_handle_t motor = NULL;

static bdc_motor_handle_t motor_pool[motores_usados];
static bdc_motor_config_t motor_config[motores_usados];
static bdc_motor_mcpwm_config_t mcpwm_config[motores_usados];
static int motores_configurados = 0;

motor_t Crear_motor(u_int32_t pin_p, u_int32_t pin_n)
{
    if (motores_configurados < motores_usados)
    {
        motor_config[motores_configurados].pwm_freq_hz = BDC_MCPWM_FREQ_HZ;
        motor_config[motores_configurados].pwma_gpio_num = pin_p;
        motor_config[motores_configurados].pwmb_gpio_num = pin_n;

        mcpwm_config[motores_configurados].group_id = 0;
        mcpwm_config[motores_configurados].resolution_hz = BDC_MCPWM_TIMER_RESOLUTION_HZ;

        printf("pin de motor A:%ld\n\r", motor_config[motores_configurados].pwma_gpio_num);
        printf("pin de motor B:%ld\n\r", motor_config[motores_configurados].pwmb_gpio_num);
        ESP_LOGI(TAG, "creando motor");
        ESP_ERROR_CHECK(bdc_motor_new_mcpwm_device(&motor_config[motores_configurados], &mcpwm_config[motores_configurados], &motor_pool[motores_configurados]));
        ESP_LOGI(TAG, "Enable motor");
        ESP_ERROR_CHECK(bdc_motor_enable(motor_pool[motores_configurados]));
        ESP_LOGI(TAG, "Stop motor");
        ESP_ERROR_CHECK(bdc_motor_brake(motor_pool[motores_configurados]));
        ESP_LOGI(TAG, "motor iniciado");
        // motor_pool[motores_configurados] = motor;
        return motor_pool[motores_configurados++];
    }
    else
        return NULL;
}