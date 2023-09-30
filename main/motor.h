#pragma once
/*motor.h debe tener las funciones de encapsulamiento*/
#include <stdint.h>
#include "bdc_motor.h"

typedef bdc_motor_handle_t motor_t;

motor_t Crear_motor (u_int32_t pin_p, u_int32_t pin_n);
