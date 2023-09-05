#include <stdio.h>
#include "driver/gpio.h"
#include <freertos/FreeRTOS.h>
#include <freertos/task.h>
#include "boton.h"
#include "estructura.h" 

//configura cada boton
void button_config (struct button *boton_u, gpio_num_t pin){
    boton_u->state = button_state_up; //estado inicial UP
    boton_u->pin = pin;               //el pin usado es
    //seteo de GPIOs
    gpio_set_direction(pin, GPIO_MODE_INPUT);
    gpio_set_pull_mode(pin, GPIO_PULLUP_ONLY);

}


//actualizacion de cada boton (esta funcion se llama tres veces, una vez por cada sensor, izquierda, derecha y retroceso)
//y devuelve el boton que recibió ya configurado

void button_update (struct button *boton_u)  
{   
    int read; 
    read = gpio_get_level(boton_u->pin);  // leer el gpio del pin a la direccion especificada
    switch(boton_u->state)
    {
        case button_state_up:
            if(read == 0)
            {
                boton_u->state = button_state_fall;
                printf("lectura cayendo\n\r");
            }
            printf("lectura arriba\n\r");
        
        break;

        case button_state_down:
            if(read == 1)
            {
                boton_u->state = button_state_rise;
                printf("lectura subiendo\n\r");
            }
    

        break;

        case button_state_fall:
            if(read == 0)
            {
                boton_u->state = button_state_down;
                printf(" lectura abajo\n\r");
            }
            else{
                boton_u->state = button_state_up;
                printf("lectura arriba\n\r");
             }
        

        break;

        case button_state_rise:
        if(read == 0)
        {
            boton_u->state = button_state_down;
            printf("lectura abajo\n\r");
        }else {
            boton_u->state = button_state_up;
            printf("lectura arriba\n\r");
        }
        break;

        default:
        boton_u->state = button_state_up;
        printf("lectura arriba_default\n\r");


    }    
}
