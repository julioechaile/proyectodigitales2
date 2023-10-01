#ifndef EJE_H
#define EJE_H
#include "motor.h"
#include "bdc_motor.h"

enum estado
{
  estado_avanzar,
  estado_derecha,
  estado_izquierda,
  estado_reversa,
  estado_detenido,
};
typedef struct Eje_c *Eje_t;

// inicializa los motores y entrega un puntero a una estructura que tiene los handles de los motores
Eje_t Crear_eje(void);
void Eje_set(enum estado estado_robot, Eje_t Eje_cfg);

#endif // EJE_H
