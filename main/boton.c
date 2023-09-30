#include <stdio.h>
#include "driver/gpio.h"
#include "estructura.h"
#include "boton.h"
#include "config.h"
// configura cada boton
// esta funcion recibe un puntero a sensor, segun clual sea.
// asigna los pines recibidos y devuelve la configuracion

struct Boton
{
    enum button_state state;
    gpio_num_t pin; // direccion del pin a leer
};

static int sensores_creados = 0;

static struct Boton boton_pool[Sensores_maximos];

// funcion que crea un boton y devuelve un puntero a boton

Boton_t button_config(gpio_num_t pin)
{
    if (sensores_creados < Sensores_maximos)
    {
        boton_pool[sensores_creados].state = button_state_up; // estado inicial UP
        boton_pool[sensores_creados].pin = pin;               // el pin usado es
        // seteo de GPIOs
        gpio_set_direction(pin, GPIO_MODE_INPUT);
        gpio_set_pull_mode(pin, GPIO_PULLUP_ONLY);
        printf("sensor iniciado\n\r");
        return &boton_pool[sensores_creados++];
    }
    return 0;
}

// actualizacion de cada boton (esta funcion se llama tres veces, una vez por cada sensor, izquierda, derecha y retroceso)
// y devuelve el boton que recibió ya configurado

enum button_state button_update(Boton_t boton_u)
{
    int read;
    read = gpio_get_level(boton_u->pin); // leer el gpio del pin a la direccion especificada
    switch (boton_u->state)
    {
    case button_state_up:
        if (read == 0)
        {
            boton_u->state = button_state_fall;
            printf("lectura cayendo\n\r");
        }
        printf("lectura arriba\n\r");

        break;

    case button_state_down:
        if (read == 1)
        {
            boton_u->state = button_state_rise;
            printf("lectura subiendo\n\r");
        }

        break;

    case button_state_fall:
        if (read == 0)
        {
            boton_u->state = button_state_down;
            printf(" lectura abajo\n\r");
        }
        else
        {
            boton_u->state = button_state_up;
            printf("lectura arriba\n\r");
        }

        break;

    case button_state_rise:
        if (read == 0)
        {
            boton_u->state = button_state_down;
            printf("lectura abajo\n\r");
        }
        else
        {
            boton_u->state = button_state_up;
            printf("lectura arriba\n\r");
        }
        break;

    default:
        boton_u->state = button_state_up;
        printf("lectura arriba_default\n\r");
    }
    return boton_u->state;
}
