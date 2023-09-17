#ifndef EJE_H
#define EJE_H
#include "motor.h"
struct Eje
{
  bdc_motor_handle_t Motor_L;
  bdc_motor_handle_t Motor_R;
};

enum estado
{
    estado_avanzar,
    estado_derecha,
    estado_izquierda,
    estado_reversa,
    estado_detenido,
};

//inicializa los motores y entrega un puntero a una estructura que tiene los handles de los motores
struct Eje * Crear_eje(void);
void Eje_set(enum estado estado_robot, struct Eje * Eje_cfg);

#endif // EJE_H
