#ifndef EJE_H
#define EJE_H

struct Eje
{
  struct Motor * babor;
  struct Motor * estribor;
};

void Eje_config(void);
void Eje_set(robot_t * robot_e);

#endif // EJE_H
