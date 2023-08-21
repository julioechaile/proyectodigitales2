#include <stdio.h>
#include "driver/gpio.h"
#include <freertos/FreeRTOS.h>
#include <freertos/task.h>
#include "lectura.h"

//actualizacion de cada boton (esta funcion se llama tres veces, una vez por cada sensor, izquierda, derecha y retroceso)
//y devuelve el boton que recibió ya configurado

struct button button_update (struct button boton_u)  
{   
    int read; 
    read = gpio_get_level(boton_u.pin);  // leer el gpio del pin a la direccion especificada
    switch(boton_u.state)
    {
        case button_state_up:
            if(read == 0)
            {
                boton_u.state = button_state_fall;
                printf("lectura cayendo\n\r");
            }
            printf("lectura arriba\n\r");
            return boton_u;
        break;

        case button_state_down:
            if(read == 1)
            {
                boton_u.state = button_state_rise;
                printf("lectura subiendo\n\r");
            }
            return boton_u;

        break;

        case button_state_fall:
            if(read == 0)
            {
                boton_u.state = button_state_down;
                printf(" lectura abajo\n\r");
            }
            else{
                boton_u.state = button_state_up;
                printf("lectura arriba\n\r");
             }
            return boton_u;

        break;

        case button_state_rise:
        if(read == 0)
        {
            boton_u.state = button_state_down;
            printf("lectura abajo\n\r");
        }else {
            boton_u.state = button_state_up;
            printf("lectura arriba\n\r");
        }
        return boton_u;
        break;

        default:
        boton_u.state = button_state_up;
        printf("lectura arriba_default\n\r");
        return boton_u;
        break;


    }    
}
