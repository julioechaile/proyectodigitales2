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
#define sensor_der GPIO_NUM_4
#define sensor_ret GPIO_NUM_0

TaskHandle_t Handle = NULL;

enum estado
{
    estado_avanzar,
    estado_derecha,
    estado_izquierda,
    estado_reversa,
};

struct entradas
{
    int izq;
    int der;
    int ret;
};

struct salidas
{
    bool mot1;
    bool mot2;
    bool mota;
    bool motb;
};
struct direcciones
{
    int dir_izq;
    int dir_der;
    int dir_ret;
};
struct robot
{
    enum estado status;
    struct entradas sensor;
    struct salidas motor;
    struct direcciones add;
};

void init(struct robot *robot_init) // aca la maquina se llama robot_init
{
    printf("Iniciando robot\n\r");

    robot_init->status = estado_avanzar;
    robot_init->motor.mot1 = 1;
    robot_init->motor.mot2 = 0;
    robot_init->motor.mota = 1;
    robot_init->motor.motb = 0;

    robot_init->sensor.izq = 1;
    robot_init->sensor.der = 1;
    robot_init->sensor.ret = 1;
    printf("sensor izq inicio: %d\n\r", robot_init->sensor.izq);
    printf("sensor der inicio: %d\n\r", robot_init->sensor.der);
    printf("sensor ret inicio: %d\n\r", robot_init->sensor.ret);

    // robot_init->add.dir_izq = sensor_izq;
    // robot_init->add.dir_der = sensor_der;
    // robot_init->add.dir_ret = sensor_ret;

    gpio_set_direction(sensor_izq, GPIO_MODE_INPUT);
    gpio_set_pull_mode(sensor_izq, GPIO_PULLUP_ONLY);
    gpio_set_direction(sensor_der, GPIO_MODE_INPUT);
    gpio_set_pull_mode(sensor_der, GPIO_PULLUP_ONLY);
    gpio_set_direction(sensor_ret, GPIO_MODE_INPUT);
    gpio_set_pull_mode(sensor_ret, GPIO_PULLUP_ONLY);
}

void robot_update(struct robot *robot_update) // aca la maquina se llama robot_update
{
    printf("Actualizando robot\n\r");
    robot_update->sensor.izq = gpio_get_level(sensor_izq); // robot_update->add.dir_izq);
    robot_update->sensor.der = gpio_get_level(sensor_der); // robot_update->add.dir_der);
    robot_update->sensor.ret = gpio_get_level(sensor_ret); // robot_update->add.dir_ret);
    printf("sensor izq: %d\n\r", robot_update->sensor.izq);
    printf("sensor der: %d\n\r", robot_update->sensor.der);
    printf("sensor ret: %d\n\r", robot_update->sensor.ret);

    switch (robot_update->status)
    { // revisa los estados posibles del robot
    case estado_avanzar:
        if (robot_update->sensor.izq == 0)
        {
            robot_update->status = estado_izquierda;
        }
        else if (robot_update->sensor.der == 0)
        {
            robot_update->status = estado_derecha;
        }
        else if (robot_update->sensor.ret == 0)
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

        if (robot_update->sensor.der == 0)
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

        if (robot_update->sensor.izq == 0)
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
        if (robot_update->sensor.ret == 0)
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

void TestTask(void *notUsed) // llama a update para actualizar la maq
{
    int contador = 0;
    struct robot maquinas;
    while (1)
    {
        robot_update(&maquinas);
        vTaskDelay(1000 / portTICK_PERIOD_MS);
        printf("tick %d\n\r", contador);
        contador++;
    }

    vTaskDelete(NULL);
    return;
}

void TestCreate(void) // llama a init y crea la tarea TestTask
{
    struct robot maquina;
    init(&maquina);
    printf("[TEST] Creating task\n\r");
    xTaskCreate(TestTask, "test", 4096, NULL, 10, &Handle);
}
