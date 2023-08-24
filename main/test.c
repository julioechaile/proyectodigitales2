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
robot_t robot_op= NULL;


//inicializa todas las variables del robot
void robot_init(robot_t *robot_i)
{
    printf("Iniciando robot\n\r");
    //inicializo estado del robot
    robot_i->status = estado_avanzar;

    //inicializo salidas de motores
    robot_i ->motor.mot1 = 1;
    robot_i ->motor.mot2 = 0;
    robot_i ->motor.mota = 1;
    robot_i ->motor.motb = 0;
    
    //inicializo sensores
    robot_i ->sensor_derecha.state = button_state_up;
    robot_i ->sensor_derecha.pin = sensor_der; 
    robot_i ->sensor_izquierda.state = button_state_up;
    robot_i ->sensor_izquierda.pin = sensor_izq; 
    robot_i ->sensor_retroceso.state = button_state_up;
    robot_i ->sensor_retroceso.pin = sensor_ret; 

    //seteo de GPIOs
    gpio_set_direction(sensor_izq, GPIO_MODE_INPUT);
    gpio_set_pull_mode(sensor_izq, GPIO_PULLUP_ONLY);
    gpio_set_direction(sensor_der, GPIO_MODE_INPUT);
    gpio_set_pull_mode(sensor_der, GPIO_PULLUP_ONLY);
    gpio_set_direction(sensor_ret, GPIO_MODE_INPUT);
    gpio_set_pull_mode(sensor_ret, GPIO_PULLUP_ONLY);
}


//actualizacion del robot, esta funcion se llama cada un segundo desde el task, para actualizar el robot
void robot_update(robot_t *robot_u)
{
    printf("Actualizando robot\n\r");
    //llamo tres veces a update, que está en lectura.c
    robot_u ->sensor_derecha = button_update(robot_u->sensor_derecha);
    robot_u ->sensor_izquierda = button_update(robot_u->sensor_izquierda);
    robot_u ->sensor_retroceso = button_update(robot_u->sensor_retroceso);


    switch (robot_u->status)
    { // revisa los estados posibles del robot
    case estado_avanzar:

        if (robot_u->sensor_izquierda.state == button_state_down)
        {
            robot_u->status = estado_izquierda;
        }
        else if (robot_u->sensor_derecha.state == button_state_down)
        {
            robot_u->status = estado_derecha;
        }
        else if (robot_u->sensor_retroceso.state == button_state_down)
        {
            robot_u->status = estado_reversa;
        }
        //le mando la estructura robot con el estado seteado, para que setee los motores
        setear_eje(&robot_u); 

        break;
    case estado_derecha:

        setear_eje(&robot_u);

        if (robot_u->sensor_derecha.state == button_state_down)
        {
            break;
        }
        else
        {
            robot_u->status = estado_avanzar;
        }
        break;
    case estado_izquierda:

        setear_eje(&robot_u);

        if (robot_u->sensor_izquierda.state == button_state_down)
        {
            break;
        }
        else
        {
            robot_u->status = estado_avanzar;
        }
        break;
    case estado_reversa:

        setear_eje(&robot_u);
        if (robot_u->sensor_retroceso.state == button_state_down)
        {
            break;
        }
        else
        {
            robot_u->status = estado_avanzar;
        }
        break;
    default:
        robot_u->status = estado_avanzar;
        setear_eje(&robot_u);
        break;
    }
}

//tarea que inicializa  y luego actualiza al robot cada un segundo
void TestTask(void *notUsed)
{
    int contador = 0;
    
    robot_init(&robot_op); 
    while (1)
    {
        robot_update(&robot_op);
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
