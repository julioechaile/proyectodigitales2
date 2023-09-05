#include <stdio.h>
#include <stdbool.h>
#include <unistd.h>
#include "driver/gpio.h"
#include <freertos/FreeRTOS.h>
#include <freertos/task.h>
#include <freertos/FreeRTOSConfig_arch.h>
#include "estructura.h"
#include "boton.h"
#include "eje.h"
#include "robot.h"
#include "config.h"
#include "motor.h"

TaskHandle_t Handle = NULL;

//inicializa todas las variables del robot
void robot_init(robot_t *robot_i)
{
    printf("Iniciando robot\n\r");
    //inicializo estado del robot
    robot_i->status = estado_detenido;

    //inicializo salidas de motores
    Motor_create(&robot_i->motor.motA, PIN_P_MOTA, PIN_N_MOTA);
    Motor_create(&robot_i->motor.motb, PIN_P_MOTB, PIN_N_MOTB);
    
    //inicializo sensores
    button_config (&robot_i ->sensor_derecha, sensor_der);
    button_config (&robot_i ->sensor_izquierda, sensor_izq);
    button_config (&robot_i ->sensor_retroceso, sensor_ret);
    
    //inicializo eje
    Eje_create(/* configuración */);
}


//actualizacion del robot, esta funcion se llama cada un segundo desde el task, para actualizar el robot
void robot_update(robot_t *robot_u)
{
    printf("Actualizando robot\n\r");
    //llamo tres veces a update, que está en boton.c
    button_update(&robot_u->sensor_derecha);
    button_update(&robot_u->sensor_izquierda);
    button_update(&robot_u->sensor_retroceso);


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
        Eje_set(robot_u); 

        break;
    case estado_derecha:

        Eje_set(robot_u);

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

        Eje_set(robot_u);

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

        Eje_set(robot_u);
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
        Eje_set(robot_u);
        break;
    }
}

//tarea que inicializa  y luego actualiza al robot cada un segundo
void RobotTask(void *notUsed)
{
    int contador = 0;
    robot_t robot_task;
    robot_init(&robot_task);  //desde acá, el puntero que apunta a robot_task pasa a robot_int 
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
    printf("creando tarea\n\r");
    xTaskCreate(RobotTask, "robot", 2048, NULL, 10, &Handle);  // crea TestTask
}
