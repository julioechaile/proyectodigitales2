#pragma once
/*motor.h debe tener las funciones de encapsulamiento*/
#include <stdint.h>
#include "bdc_motor.h"

bdc_motor_handle_t Crear_motor (gpio_num_t pin_p, gpio_num_t pin_n);
