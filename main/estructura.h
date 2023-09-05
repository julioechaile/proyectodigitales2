#include <stdint.h>
#include "driver/gpio.h"

//creacion de la maquina de estados del boton

enum button_state      {
    button_state_up,
    button_state_fall,
    button_state_down,
    button_state_rise,
};


//creacion de la estructura del boton

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
    estado_detenido,
};

//creacion de la estructura del robot

struct Motor
{
  uint32_t pin_p;
  uint32_t pin_n;
  uint32_t duty;
};

struct salidas
{
    struct Motor motA;
    struct Motor motB;
};

struct robot
{
    enum estado status;
    struct button sensor_derecha;
    struct button sensor_izquierda;
    struct button sensor_retroceso;
    struct salidas motor;
};
typedef struct robot robot_t;