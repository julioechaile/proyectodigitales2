#include <stdint.h>
#include <stdio.h>
#include <unistd.h>
#include <freertos/FreeRTOS.h>
#include "driver/gpio.h"
#include <freertos/task.h>
#include "freertos/queue.h"
#include <freertos/FreeRTOSConfig_arch.h>
#include "boton.h"
#include "robot.h"
#include "eje.h"
#include "config.h"
#include "sensor_bat.h"

TaskHandle_t Handle = NULL;

static Eje_t Eje_cfg;
static int voltage;

// inicializa todas las variables del robot
void robot_init(robot_t *robot_i)
{
    printf("Iniciando robot\n\r");
    // inicializo estado del robot
    robot_i->status = estado_detenido;

    // inicializo sensores
    robot_i->sensor_derecha = button_config(sensor_der);
    robot_i->sensor_izquierda = button_config(sensor_izq);
    robot_i->sensor_retroceso = button_config(sensor_ret);

    // seteo el ADC1 para medir
    Set_ADC();

    // inicializo eje y devuelvo un puntero con el handle de los dos motores
    Eje_cfg = Crear_eje();
}

// actualizacion del robot, esta funcion se llama cada un segundo desde el task, para actualizar el robot
void robot_update(robot_t *robot_u)
{
    printf("Actualizando robot\n\r");
    // llamo tres veces a update, que está en boton.c
    enum button_state Status_sensor_L = button_update(robot_u->sensor_derecha);
    enum button_state Status_sensor_R = button_update(robot_u->sensor_izquierda);
    enum button_state Status_sensor_RR = button_update(robot_u->sensor_retroceso);

    // leo el voltaje de batería
    voltage = get_voltage();

    if (voltage > 1700)
    {

        switch (robot_u->status)
        {
            // revisa los estados posibles del robot
            // lo que se fija aca es el estado  de los botones
            // que fue preguntado por button_update para cada sensor
        case estado_avanzar:

            if (Status_sensor_L == button_state_down)
            {
                robot_u->status = estado_izquierda;
            }
            else if (Status_sensor_R == button_state_down)
            {
                robot_u->status = estado_derecha;
            }
            else if (Status_sensor_RR == button_state_down)
            {
                robot_u->status = estado_reversa;
            }
            // le mando la estructura robot con el estado seteado, para que setee los motores
            Eje_set(robot_u->status, Eje_cfg);

            break;
        case estado_derecha:

            Eje_set(robot_u->status, Eje_cfg);

            if (Status_sensor_R == button_state_down)
            {
                break;
            }
            else
            {
                robot_u->status = estado_avanzar;
            }
            break;
        case estado_izquierda:

            Eje_set(robot_u->status, Eje_cfg);

            if (Status_sensor_L == button_state_down)
            {
                break;
            }
            else
            {
                robot_u->status = estado_avanzar;
            }
            break;
        case estado_reversa:

            Eje_set(robot_u->status, Eje_cfg);
            if (Status_sensor_RR == button_state_down)
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
            Eje_set(robot_u->status, Eje_cfg);
            break;
        }
    }
    else
    {
        robot_u->status = estado_detenido;
        Eje_set(robot_u->status, Eje_cfg);
        printf("detenido: Bateria baja\n\r");
    }
}

// tarea que inicializa  y luego actualiza al robot cada un segundo
void RobotTask(void *notUsed)
{
    int contador = 0;
    robot_t robot_task;
    robot_init(&robot_task); // desde acá, el puntero que apunta a robot_task pasa a robot_int
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

// creo la tarea de actualizacion "Robot"

void TestCreate(void)
{
    printf("creando tarea\n\r");
    xTaskCreatePinnedToCore(RobotTask, "Robot", 4096, NULL, 10, &Handle, 1);

    // xTaskCreate(RobotTask, "robot", 4096, NULL, 9, &Handle);  // crea TestTask
}
