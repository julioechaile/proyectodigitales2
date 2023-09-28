/*el eje debe poder realizar lo siguiente:

Inicializarse (crear_eje)
    crear los motores necesarios
    entregar un puntero a eje

Recibir un estado de la logica de negocio (setear_eje) y actuar sobre los motores (motor_F,_R,B)

necesito una estructura de entrada (puede ser un enum)
*/
#include <stdio.h>
#include "driver/gpio.h"
#include "estructura.h"
#include "config.h"
#include "esp_attr.h"

#include "sdkconfig.h"
#include "esp_log.h"
#include "bdc_motor.h"
#include "motor.h"
#include "eje.h"

static const char *TAG = "eje";

struct Eje_c
{
    bdc_motor_handle_t Motor_L;
    bdc_motor_handle_t Motor_R;
};

static struct Eje_c *Eje_config;

// llama a crear motor y recibe un struct Eje_t
Eje_t Crear_eje(void)

{
    // creacion del motor_L
    Eje_config->Motor_L = Crear_motor(PIN_P_MOTA, PIN_N_MOTA);
    Eje_config->Motor_R = Crear_motor(PIN_P_MOTB, PIN_N_MOTB);
    // ya tengo los dos motores creados y configurados
    return Eje_config;
}

void Eje_set(enum estado estado_robot, Eje_t Eje_cfg)
{

    switch (estado_robot)
    {

    case estado_avanzar:

        printf("Avanzando\n\r");

        // tengo que mandarle el puntero para que sepa que motor es
        ESP_LOGI(TAG, "Forward motor");
        ESP_ERROR_CHECK(bdc_motor_forward(Eje_cfg->Motor_L));
        bdc_motor_set_speed(Eje_cfg->Motor_L, 80);
        ESP_ERROR_CHECK(bdc_motor_forward(Eje_cfg->Motor_R));
        bdc_motor_set_speed(Eje_cfg->Motor_R, 80);
        break;

    case estado_derecha:
        printf("Giro derecha\n\r");
        ESP_ERROR_CHECK(bdc_motor_forward(Eje_cfg->Motor_L));
        bdc_motor_set_speed(Eje_cfg->Motor_L, 60);
        ESP_ERROR_CHECK(bdc_motor_reverse(Eje_cfg->Motor_R));
        bdc_motor_set_speed(Eje_cfg->Motor_R, 60);

        break;

    case estado_izquierda:
        printf("Giro izquierda\n\r");
        ESP_ERROR_CHECK(bdc_motor_reverse(Eje_cfg->Motor_L));
        bdc_motor_set_speed(Eje_cfg->Motor_L, 60);
        ESP_ERROR_CHECK(bdc_motor_forward(Eje_cfg->Motor_R));
        bdc_motor_set_speed(Eje_cfg->Motor_R, 60);

        break;

    case estado_reversa:
        printf("Retrocediendo\n\r");
        ESP_ERROR_CHECK(bdc_motor_reverse(Eje_cfg->Motor_L));
        bdc_motor_set_speed(Eje_cfg->Motor_L, 80);
        ESP_ERROR_CHECK(bdc_motor_reverse(Eje_cfg->Motor_R));
        bdc_motor_set_speed(Eje_cfg->Motor_R, 80);

        break;

    case estado_detenido:
        ESP_ERROR_CHECK(bdc_motor_brake(Eje_cfg->Motor_L));
        ESP_ERROR_CHECK(bdc_motor_brake(Eje_cfg->Motor_R));
        break;

    default:
        ESP_ERROR_CHECK(bdc_motor_forward(Eje_cfg->Motor_L));
        bdc_motor_set_speed(Eje_cfg->Motor_L, 80);
        ESP_ERROR_CHECK(bdc_motor_forward(Eje_cfg->Motor_L));
        bdc_motor_set_speed(Eje_cfg->Motor_R, 80);
        printf("Avanzando por defecto\n\r");
    }
}