#include <stdio.h>
#include <freertos/FreeRTOS.h>
#include <freertos/task.h>
#include "driver/gpio.h"
#include "estructura.h"
#include "config.h"
#include "esp_attr.h"

#include "sdkconfig.h"

#include "freertos/queue.h"
#include "esp_log.h"
#include "esp_timer.h"
#include "eje.h"
#include "motor.h"


#define BDC_MCPWM_TIMER_RESOLUTION_HZ 10000000 // 10MHz, 1 tick = 0.1us
#define BDC_MCPWM_FREQ_HZ             25000    // 25KHz PWM
#define BDC_MCPWM_DUTY_TICK_MAX       (BDC_MCPWM_TIMER_RESOLUTION_HZ / BDC_MCPWM_FREQ_HZ) // maximum value we can set for the duty cycle, in ticks

static const char *TAG = "eje";

/*eje.c recibe los estados del robot y crea dos motores.
luego comanda esos motores segun la accion que se necesite*/

/*variables globales*/
bdc_motor_handle_t Motor_L;
bdc_motor_handle_t Motor_R;
bdc_motor_handle_t motor;

//por ahora crea los motores de forma secuencial
void Eje_config(void){

    //creacion del motor_L
    ESP_LOGI(TAG, "Create DC motor_L");
    bdc_motor_config_t motor_config = {
        .pwm_freq_hz = BDC_MCPWM_FREQ_HZ,
        .pwma_gpio_num = PIN_P_MOTA,
        .pwmb_gpio_num = PIN_N_MOTA,
    };
    bdc_motor_mcpwm_config_t mcpwm_config = {
        .group_id = 0,
        .resolution_hz = BDC_MCPWM_TIMER_RESOLUTION_HZ,
    };

    //pone el handle motor en null
    motor = NULL;
    ESP_ERROR_CHECK(bdc_motor_new_mcpwm_device(&motor_config, &mcpwm_config, &motor));
    Motor_L = motor;

    ESP_LOGI(TAG, "Enable motor_L");
    ESP_ERROR_CHECK(bdc_motor_enable(motor));

    //ahora creacion del motor_R
    ESP_LOGI(TAG, "Create DC motor_R");
    
    //Seteo la estructura con los nuevos valores
    motor_config.pwm_freq_hz = BDC_MCPWM_FREQ_HZ;
    //los GPIO son de config.h
    motor_config.pwma_gpio_num = PIN_P_MOTB;
    motor_config.pwmb_gpio_num = PIN_N_MOTB;

    //seteo la estructura con nuevos valores
    mcpwm_config.group_id = 0;
    mcpwm_config.resolution_hz = BDC_MCPWM_TIMER_RESOLUTION_HZ;

    //pone el handle motor en null otra vez
    bdc_motor_handle_t motor = NULL;
    ESP_ERROR_CHECK(bdc_motor_new_mcpwm_device(&motor_config, &mcpwm_config, &motor));
    Motor_R = motor;

    ESP_LOGI(TAG, "Enable motor_R");
    ESP_ERROR_CHECK(bdc_motor_enable(motor));
    //ya tengo los dos motores creados y configurados
}

//se implementará a futuro con puntero opaco...
void Eje_set(robot_t *robot_e){

    switch(robot_e->status){

        case estado_avanzar:
            
            printf("Avanzando\n\r");

            //tengo que mandarle el puntero para que sepa que motor es
            ESP_LOGI(TAG, "Forward motor");
            ESP_ERROR_CHECK(bdc_motor_forward(Motor_L));
            bdc_motor_set_speed(Motor_L, 80);
            ESP_ERROR_CHECK(bdc_motor_forward(Motor_L));
            bdc_motor_set_speed(Motor_R, 80);
            break;

        case estado_derecha:
            printf("Giro derecha\n\r");
            ESP_ERROR_CHECK(bdc_motor_forward(Motor_L));
            bdc_motor_set_speed(Motor_L, 60);
            ESP_ERROR_CHECK(bdc_motor_reverse(Motor_R));
            bdc_motor_set_speed(Motor_R, 60);
               
            break;

        case estado_izquierda:
            printf("Giro izquierda\n\r");
            ESP_ERROR_CHECK(bdc_motor_reverse(Motor_L));
            bdc_motor_set_speed(Motor_L, 60);
            ESP_ERROR_CHECK(bdc_motor_forward(Motor_R));
            bdc_motor_set_speed(Motor_R, 60);
            
            break;

        case estado_reversa:
            printf("Retrocediendo\n\r");
            ESP_ERROR_CHECK(bdc_motor_reverse(Motor_L));
            bdc_motor_set_speed(Motor_L, 80);
            ESP_ERROR_CHECK(bdc_motor_reverse(Motor_R));
            bdc_motor_set_speed(Motor_R, 80);
            
            break;
            
        case estado_detenido:
            ESP_ERROR_CHECK(bdc_motor_brake(Motor_L));
            ESP_ERROR_CHECK(bdc_motor_brake(Motor_R));
            break;
            
        default:
            ESP_ERROR_CHECK(bdc_motor_forward(Motor_L));
            bdc_motor_set_speed(Motor_L, 80);
            ESP_ERROR_CHECK(bdc_motor_forward(Motor_L));
            bdc_motor_set_speed(Motor_R, 80);
            printf("Avanzando por defecto\n\r");
       
    }

}