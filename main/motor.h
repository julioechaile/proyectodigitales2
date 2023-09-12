
#ifndef MOTOR_H
#define MOTOR_H
//estados del motor
//enum Motor_direction
//{
//  Motor_direction_stop,
//  Motor_direction_clockwise,
//  Motor_direction_counterClockwise
//};

//estructura mediadora
struct motor_config{
    gpio_num_t pin_p;
    gpio_num_t pin_n;
    mcpwm_timer_t timer;
    mcpwm_unit_t unit;

};
typedef struct Motor* Motor_t;

Motor_t mcpwm_gpio_initialize(struct motor_config * motor_c);
static void brushed_motor_forward(Motor_t config, float duty_cycle);
static void brushed_motor_backward(Motor_t config, float duty_cycle);
static void brushed_motor_stop(Motor_t config);
#endif
