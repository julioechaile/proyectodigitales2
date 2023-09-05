#include <stdio.h>
#include <freertos/FreeRTOS.h>
#include <freertos/task.h>
#include "driver/gpio.h"
#include "estructura.h"
#include "eje.h"
#include "motor.h"

void Eje_config(robot_t * robot_e){

Motor_config(robot_e->motor.motA, PIN_P_MOTA, PIN_N_MOTA);
Motor_config(robot_e->motor.motb, PIN_P_MOTB, PIN_N_MOTB);

}
void Eje_set(robot_t *robot_e){

switch(robot_e->status){

        case estado_avanzar:
           //deberia llamar a esta funcion: 
           //Motor_set(struct Motor * motor, enum Motor_direction direction, uint32_t duty);  
           //falta ver PWM en esp32...

           // robot_op->motor.mot1 = 1;   NO
           // robot_op->motor.mot2 = 0;   NO
           // robot_op->motor.mota = 1;   NO
           // robot_op->motor.motb = 0;   NO
            printf("Avanzando\n\r");
            break;

        case estado_derecha:
            //robot_op->motor.mot1 = 1;
            //robot_op->motor.mot2 = 0;
            //robot_op->motor.mota = 0;
            //robot_op->motor.motb = 1;
            printf("Giro derecha\n\r");   
            break;

        case estado_izquierda:

            //robot_op -> motor.mot1 = 0;
            //robot_op -> motor.mot2 = 1;
            //robot_op -> motor.mota = 1;
            //robot_op -> motor.motb = 0;
            printf("Giro izquierda\n\r");
            break;

        case estado_reversa:
            //robot_op -> motor.mot1 = 0;
            //robot_op -> motor.mot2 = 1;
            //robot_op -> motor.mota = 0;
            //robot_op -> motor.motb = 1;
            //printf("Retrocediendo\n\r");
            break;
            
        default:
            robot_op -> motor.mot1 = 1;
            robot_op -> motor.mot2 = 0;
            robot_op -> motor.mota = 1;
            robot_op -> motor.motb = 0;
            printf("Avanzando\n\r");
            break;   
    }


}
