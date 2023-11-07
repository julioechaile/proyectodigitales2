#pragma once
/*motor.h debe tener las funciones de encapsulamiento*/
#include <stdint.h>
#include "bdc_motor.h"

typedef bdc_motor_handle_t motor_t;

motor_t Crear_motor (uint32_t pin_p, uint32_t pin_n);

void Set_motor_forward(motor_t motor, uint32_t duty);
void Set_motor_backward(motor_t motor, uint32_t duty);
void Set_motor_brake(motor_t motor);

