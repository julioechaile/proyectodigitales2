#include <stdio.h>
#include "driver/gpio.h"
#include <freertos/FreeRTOS.h>
#include <freertos/task.h>
#include "salida.h"
#include "estructura.h"
//extern robot_t robot_u;


void setear_eje(robot_t self){

    switch (self->status){

        case estado_avanzar:
              
            self->motor.mot1 = 1;
            self->motor.mot2 = 0;
            self->motor.mota = 1;
            self->motor.motb = 0;
            printf("Avanzando\n\r");
            break;

        case estado_derecha:
            self->motor.mot1 = 1;
            self->motor.mot2 = 0;
            self->motor.mota = 0;
            self->motor.motb = 1;
            printf("Giro derecha\n\r");   
            break;

        case estado_izquierda:

            self->motor.mot1 = 0;
            self->motor.mot2 = 1;
            self->motor.mota = 1;
            self->motor.motb = 0;
            printf("Giro izquierda\n\r");
            break;

        case estado_reversa:
            self->motor.mot1 = 0;
            self->motor.mot2 = 1;
            self->motor.mota = 0;
            self->motor.motb = 1;
            printf("Retrocediendo\n\r");
            break;
            
        case default:
            self->motor.mot1 = 1;
            self->motor.mot2 = 0;
            self->motor.mota = 1;
            self->motor.motb = 0;
            printf("Avanzando\n\r");
            break;   
    }
}