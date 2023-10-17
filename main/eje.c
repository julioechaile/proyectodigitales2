/*el eje debe poder realizar lo siguiente:

Inicializarse (crear_eje)
    crear los motores necesarios
    entregar un puntero a eje

Recibir un estado de la logica de negocio (setear_eje) y actuar sobre los motores (motor_F,_R,B)

necesito una estructura de entrada (puede ser un enum)

*/
#include <stdio.h>
#include "estructura.h"
#include "config.h"
#include "esp_attr.h"

#include "sdkconfig.h"
#include "esp_log.h"
#include "eje.h"
#include "motor.h"
#include "bdc_motor.h"

static const char *TAG = "eje";

struct Eje_c
{
    motor_t Motor_L;
    motor_t Motor_R;
};

static struct Eje_c Eje_config;

static motor_t Motor_PL;
static motor_t Motor_PR;

// llama a crear motor y recibe un struct Eje_t
Eje_t Crear_eje(void)
{
    // Eje_config->Motor_L = NULL;
    // Eje_config->Motor_R = NULL;

    // creacion del motor_L
    // Eje_config->Motor_L = Crear_motor(PIN_P_MOTA, PIN_N_MOTA);

    Motor_PL = Crear_motor(PIN_P_MOTA, PIN_N_MOTA);
    

    // creacion del motor_R
    // Eje_config->Motor_R = Crear_motor(PIN_P_MOTB, PIN_N_MOTB);
    Motor_PR = Crear_motor(PIN_P_MOTB, PIN_N_MOTB);
    // ya tengo los dos motores creados y configurados

    Eje_config.Motor_L = Motor_PL;
    Eje_config.Motor_R = Motor_PR;
    return &Eje_config;
}

void Eje_set(enum estado estado_robot, Eje_t Eje_cfg)
{

    switch (estado_robot)
    {

    case estado_avanzar:

        printf("Avanzando\n\r");

        // tengo que mandarle el puntero para que sepa que motor es
        ESP_LOGI(TAG, "Avanzando");

        Set_motor_forward(Eje_cfg->Motor_L, 80);
        Set_motor_forward(Eje_cfg->Motor_R, 80);
        break;

    case estado_derecha:

        ESP_LOGI(TAG, "Giro Derecha");

        Set_motor_forward(Eje_cfg->Motor_L, 80);
        Set_motor_backward(Eje_cfg->Motor_R, 60);

        break;

    case estado_izquierda:
        ESP_LOGI(TAG, "Giro Izquierda");

        Set_motor_backward(Eje_cfg->Motor_L, 60);
        Set_motor_forward(Eje_cfg->Motor_R, 80);

        break;

    case estado_reversa:

        ESP_LOGI(TAG, "Retrocediendo");

        Set_motor_backward(Eje_cfg->Motor_L, 80);
        Set_motor_backward(Eje_cfg->Motor_R, 80);

        break;

    case estado_detenido:
        ESP_LOGI(TAG, "Detenido");
        Set_motor_brake(Eje_cfg->Motor_L);
        Set_motor_brake(Eje_cfg->Motor_R);

        break;

    default:
        Set_motor_forward(Eje_cfg->Motor_L, 80);
        Set_motor_forward(Eje_cfg->Motor_R, 80);
        ESP_LOGI(TAG, "Avanzando por defecto");
    }
}