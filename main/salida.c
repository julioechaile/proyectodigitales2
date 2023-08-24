#include <stdio.h>
#include <freertos/FreeRTOS.h>
#include <freertos/task.h>
#include "driver/gpio.h"
#include "estructura.h"
//#include "salida.h"
void setear_eje(robot_t robot_op);
extern robot_t *robot_op;

void setear_eje(robot_t robot_op){

    switch (robot_op -> status){

        case estado_avanzar:
              
            robot_op -> motor.mot1 = 1;
            robot_op -> motor.mot2 = 0;
            robot_op -> motor.mota = 1;
            robot_op -> motor.motb = 0;
            printf("Avanzando\n\r");
            break;

        case estado_derecha:
            robot_op -> motor.mot1 = 1;
            robot_op -> motor.mot2 = 0;
            robot_op -> motor.mota = 0;
            robot_op -> motor.motb = 1;
            printf("Giro derecha\n\r");   
            break;

        case estado_izquierda:

            robot_op -> motor.mot1 = 0;
            robot_op -> motor.mot2 = 1;
            robot_op -> motor.mota = 1;
            robot_op -> motor.motb = 0;
            printf("Giro izquierda\n\r");
            break;

        case estado_reversa:
            robot_op -> motor.mot1 = 0;
            robot_op -> motor.mot2 = 1;
            robot_op -> motor.mota = 0;
            robot_op -> motor.motb = 1;
            printf("Retrocediendo\n\r");
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