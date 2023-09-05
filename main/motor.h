#include <stdint.h>
#ifndef MOTOR_H
#define MOTOR_H
struct Motor
{
  uint32_t pin_p;
  uint32_t pin_n;
  uint32_t duty;
};

enum Motor_direction
{
  Motor_direction_stop,
  Motor_direction_clockwise,
  Motor_direction_counterClockwise
};

struct Motor * Motor_config(struct Motor *motor, uint32_t pin_p, uint32_t pin_n);

void Motor_set(struct Motor * motor, enum Motor_direction direction, uint32_t duty);

#endif // MOTOR_H
