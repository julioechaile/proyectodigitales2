#include "test.h"
#include <stdio.h>
#include <stdbool.h>
#include <unistd.h>
#include "driver/gpio.h"
#include <freertos/FreeRTOS.h>
#include <freertos/task.h>
#include <stdio.h>
#include <freertos/FreeRTOSConfig_arch.h>
#include "driver/gpio.h"
#define sensor_izq GPIO_NUM_15
#define sensor_der GPIO_NUM_2
#define sensor_ret GPIO_NUM_0

TaskHandle_t Handle = NULL;

//creacion de los estados de la maquina de estados

enum button_state      {
    button_state_up,
    button_state_fall,
    button_state_down,
    button_state_rise,
};


//creacion de la estructura

struct button          
{
    enum button_state state;
    gpio_num_t pin;     //direccion del pin a leer
};

//creacion de la maquina de estados del robot
enum estado
{
    estado_avanzar,
    estado_derecha,
    estado_izquierda,
    estado_reversa,
};

//creacion de la estructura del robot

struct salidas
{
    bool mot1;
    bool mot2;
    bool mota;
    bool motb;
};

struct robot
{
    enum estado status;
    struct button sensor_derecha;
    struct button sensor_izquierda;
    struct button sensor_retroceso;
    struct salidas motor;
};


//inicializa todas las variables del robot
void robot_init(struct robot *robot_init)
{
    printf("Iniciando robot\n\r");
    //inicializo estado del robot
    robot_init->status = estado_avanzar;

    //inicializo salidas de motores
    robot_init->motor.mot1 = 1;
    robot_init->motor.mot2 = 0;
    robot_init->motor.mota = 1;
    robot_init->motor.motb = 0;
    
    //inicializo sensores
    robot_init->sensor_derecha.state = button_state_up;
    robot_init ->sensor_derecha.pin = sensor_der; 
    robot_init->sensor_izquierda.state = button_state_up;
    robot_init ->sensor_izquierda.pin = sensor_izq; 
    robot_init->sensor_retroceso.state = button_state_up;
    robot_init ->sensor_retroceso.pin = sensor_ret; 

    //seteo de GPIOs
    gpio_set_direction(sensor_izq, GPIO_MODE_INPUT);
    gpio_set_pull_mode(sensor_izq, GPIO_PULLUP_ONLY);
    gpio_set_direction(sensor_der, GPIO_MODE_INPUT);
    gpio_set_pull_mode(sensor_der, GPIO_PULLUP_ONLY);
    gpio_set_direction(sensor_ret, GPIO_MODE_INPUT);
    gpio_set_pull_mode(sensor_ret, GPIO_PULLUP_ONLY);
}


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

//actualizacion del robot, esta funcion se llama cada un segundo desde el task, para actualizar el robot
void robot_update(struct robot *robot_update)
{
    printf("Actualizando robot\n\r");

    robot_update->sensor_derecha = button_update(robot_update->sensor_derecha);
    robot_update->sensor_izquierda = button_update(robot_update->sensor_izquierda);
    robot_update->sensor_retroceso = button_update(robot_update->sensor_retroceso);


    switch (robot_update->status)
    { // revisa los estados posibles del robot
    case estado_avanzar:
        if (robot_update->sensor_izquierda.state == button_state_down)
        {
            robot_update->status = estado_izquierda;
        }
        else if (robot_update->sensor_derecha.state == button_state_down)
        {
            robot_update->status = estado_derecha;
        }
        else if (robot_update->sensor_retroceso.state == button_state_down)
        {
            robot_update->status = estado_reversa;
        }

        robot_update->motor.mot1 = 1;
        robot_update->motor.mot2 = 0;
        robot_update->motor.mota = 1;
        robot_update->motor.motb = 0;
        printf("Avanzando\n\r");
        break;
    case estado_derecha:
        robot_update->motor.mot1 = 1;
        robot_update->motor.mot2 = 0;
        robot_update->motor.mota = 0;
        robot_update->motor.motb = 1;
        printf("Giro derecha\n\r");

        if (robot_update->sensor_derecha.state == button_state_down)
        {
            break;
        }
        else
        {
            robot_update->status = estado_avanzar;
        }
        break;
    case estado_izquierda:

        robot_update->motor.mot1 = 0;
        robot_update->motor.mot2 = 1;
        robot_update->motor.mota = 1;
        robot_update->motor.motb = 0;
        printf("Giro izquierda\n\r");

        if (robot_update->sensor_izquierda.state == button_state_down)
        {
            break;
        }
        else
        {
            robot_update->status = estado_avanzar;
        }
        break;
    case estado_reversa:
        robot_update->motor.mot1 = 0;
        robot_update->motor.mot2 = 1;
        robot_update->motor.mota = 0;
        robot_update->motor.motb = 1;
        printf("Retrocediendo\n\r");
        if (robot_update->sensor_retroceso.state == button_state_down)
        {
            break;
        }
        else
        {
            robot_update->status = estado_avanzar;
        }
        break;
    default:
        robot_update->status = estado_avanzar;
        break;
    }
}

//tarea que inicializa  y luego actualiza al robot cada un segundo
void TestTask(void *notUsed)
{
    int contador = 0;
    struct robot robot_task;
    robot_init(&robot_task); 
    while (1)
    {
        robot_update(&robot_task);
        vTaskDelay(1000 / portTICK_PERIOD_MS);
        printf("tick %d\n\r", contador);
        contador++;
    }

    vTaskDelete(NULL);
    return;
}

//creo la tarea de actualizacion "test"

void TestCreate(void) 
{
    printf("[TEST] creando tarea\n\r");
    xTaskCreate(TestTask, "test", 4096, NULL, 10, &Handle);  // crea TestTask
}
