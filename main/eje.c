#include <stdio.h>
#include <freertos/FreeRTOS.h>
#include <freertos/task.h>
#include "driver/gpio.h"
#include "estructura.h"
#include "config.h"
#include "esp_attr.h"

#include "driver/mcpwm.h"
#include "soc/mcpwm_periph.h"

//#include "driver/mcpwm_prelude.h"

#include "motor.h"
#include "eje.h"

/*variables globales*/
//eje.c es mi controlador
//creo los punteros que van a recibir lo apuntado por los objetos ya configurados
//estos si son punteros
Motor_t Motor_L;
Motor_t Motor_R;

//creo la primer estructura de configuracion
//no es un puntero

struct motor_config Motor1_config;
Motor1_config.pin_p = PIN_P_MOTA;
Motor1_config.pin_n = PIN_N_MOTA;

//creo la segunda estructura de configuracion
//no es un puntero
struct motor_config Motor2_config;
Motor2_config.pin_p = PIN_P_MOTB;
Motor2_config.pin_n = PIN_N_MOTB;

//Eje_config inicializa el eje y crea los objetos motor
void Eje_config(void){

    Motor_L = mcpwm_gpio_initialize(&Motor1_config);
    Motor_R = mcpwm_gpio_initialize(&Motor2_config);

}
//se implementará a futuro con puntero opaco...
void Eje_set(robot_t *robot_e){

    switch(robot_e->status){

        case estado_avanzar:
            
            printf("Avanzando\n\r");

            //tengo que mandarle el puntero para que sepa que motor es
            brushed_motor_forward(Motor_L, 80.0);
            brushed_motor_forward(Motor_R, 80.0);
            break;

        case estado_derecha:
            brushed_motor_forward(Motor_L, 80.0);
            brushed_motor_backward(Motor_R, 80.0);
            printf("Giro derecha\n\r");   
            break;

        case estado_izquierda:
            brushed_motor_backward(Motor_L, 80.0);
            brushed_motor_forward(Motor_R, 80.0);
            printf("Giro izquierda\n\r");
            break;

        case estado_reversa:
            brushed_motor_backward(Motor_L, 60.0);
            brushed_motor_backward(Motor_R, 60.0);
            printf("Retrocediendo\n\r");
            break;
            
        case estado_detenido:
            brushed_motor_stop(Motor_L);
            brushed_motor_stop(Motor_R);
            break;
            
        default:
            brushed_motor_forward(Motor_L, 80.0);
            brushed_motor_forward(Motor_R, 80.0);
            printf("Avanzando\n\r");
       
    }

}