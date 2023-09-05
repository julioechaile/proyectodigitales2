#include"motor.h"
#include "driver/gpio.h"

void Motor_config(struct Motor *motor, uint32_t pin_a, uint32_t pin_b){
    motor->pin_p = pin_a;
    motor->pin_n = pin_b;
    gpio_set_direction(pin_a, GPIO_MODE_OUTPUT);
    gpio_set_level(pin_a,0);
    gpio_set_direction(pin_b, GPIO_MODE_OUTPUT);
    gpio_set_level(pin_b,0);

}

void Motor_set(struct Motor * motor, enum Motor_direction direction, uint32_t duty){
    //saco por los gpio las salidas
    //seteo el PWM

}