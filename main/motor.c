#include "motor.h"

void Motor_create(struct Motor *motor, uint32_t pin_a, uint32_t pin_b){
    motor->pin_p = pin_a;
    motor->pin_n = pin_b;
}

void Motor_set(struct Motor * motor, enum Motor_direction direction, uint32_t duty){
    //saco por los qpio las salidas
    //seteo el PWM

}