#ifndef EJE_H
#define EJE_H

struct Eje
{
  struct Motor * babor;
  struct Motor * estribor;
};

struct Eje * Eje_create(/* configuración */);
void Eje_set(enum estado * estado_eje);

#endif // EJE_H
