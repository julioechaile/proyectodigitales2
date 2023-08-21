#include "test.h"
#include <stdio.h>
#include <stdbool.h>
#include <unistd.h>
#include "driver/gpio.h"
#include <freertos/FreeRTOS.h>
#include <freertos/task.h>
#include <freertos/FreeRTOSConfig_arch.h>
#include "lectura.h"
#include "salida.h"
#include "estructura.h"
#define sensor_izq GPIO_NUM_15
#define sensor_der GPIO_NUM_2
#define sensor_ret GPIO_NUM_0

TaskHandle_t Handle = NULL;



//inicializa todas las variables del robot
void robot_init(robot_t self)
{
    printf("Iniciando robot\n\r");
    //inicializo estado del robot
    self->status = estado_avanzar;

    //inicializo salidas de motores
    self->motor.mot1 = 1;
    self->motor.mot2 = 0;
    self->motor.mota = 1;
    self->motor.motb = 0;
    
    //inicializo sensores
    self->sensor_derecha.state = button_state_up;
    self ->sensor_derecha.pin = sensor_der; 
    self->sensor_izquierda.state = button_state_up;
    self ->sensor_izquierda.pin = sensor_izq; 
    self->sensor_retroceso.state = button_state_up;
    self ->sensor_retroceso.pin = sensor_ret; 

    //seteo de GPIOs
    gpio_set_direction(sensor_izq, GPIO_MODE_INPUT);
    gpio_set_pull_mode(sensor_izq, GPIO_PULLUP_ONLY);
    gpio_set_direction(sensor_der, GPIO_MODE_INPUT);
    gpio_set_pull_mode(sensor_der, GPIO_PULLUP_ONLY);
    gpio_set_direction(sensor_ret, GPIO_MODE_INPUT);
    gpio_set_pull_mode(sensor_ret, GPIO_PULLUP_ONLY);
}


//actualizacion del robot, esta funcion se llama cada un segundo desde el task, para actualizar el robot
void robot_update(robot_t self)
{
    printf("Actualizando robot\n\r");
    //llamo tres veces a update, que está en lectura.c
    self->sensor_derecha = button_update(self->sensor_derecha);
    self->sensor_izquierda = button_update(self->sensor_izquierda);
    self->sensor_retroceso = button_update(self->sensor_retroceso);


    switch (self->status)
    { // revisa los estados posibles del robot
    case estado_avanzar:

        if (self->sensor_izquierda.state == button_state_down)
        {
            self->status = estado_izquierda;
        }
        else if (self->sensor_derecha.state == button_state_down)
        {
            self->status = estado_derecha;
        }
        else if (self->sensor_retroceso.state == button_state_down)
        {
            self->status = estado_reversa;
        }
        //le mando la estructura robot con el estado seteado, para que setee los motores
        setear_eje(self); 

        break;
    case estado_derecha:

        setear_eje(self);

        if (self->sensor_derecha.state == button_state_down)
        {
            break;
        }
        else
        {
            self->status = estado_avanzar;
        }
        break;
    case estado_izquierda:

        setear_eje(self);

        if (self->sensor_izquierda.state == button_state_down)
        {
            break;
        }
        else
        {
            self->status = estado_avanzar;
        }
        break;
    case estado_reversa:

        setear_eje(self);
        if (self->sensor_retroceso.state == button_state_down)
        {
            break;
        }
        else
        {
            self->status = estado_avanzar;
        }
        break;
    default:
        self->status = estado_avanzar;
        setear_eje(self);
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
