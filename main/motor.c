//se recibe de eje.c dos estructuras de configuracion de motor
//la funcion initialize devuelve dos punteros (pero hasta ahora no son necesarios...)
#include <stdio.h>

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

#include "driver/gpio.h"
//#include "driver/mcpwm_prelude.h"

#include "esp_attr.h"

#include "driver/mcpwm.h"
#include "soc/mcpwm_periph.h"
#include "motor.h"



//estructura usada para configurar al motor
struct Motor{
    gpio_num_t pin_p;
    gpio_num_t pin_n;
    mcpwm_timer_t timer;
    mcpwm_unit_t unit;

};

static struct Motor motorMemoryPool[2];//tengo dos motores para configurar
static int motorsCreated = 0;//inicialmente, ningun motor creado

//recibe la estrucutra de configuracion con los gpio deseados y el duty en 0

Motor_t mcpwm_gpio_initialize(struct motor_config * motor_c)
{
    printf("initializing mcpwm gpio...\n");
    motorMemoryPool[motorsCreated].pin_p=motor_c->pin_p;
    motorMemoryPool[motorsCreated].pin_n=motor_c->pin_n;
    motorMemoryPool[motorsCreated].unit=MCPWM_UNIT_0;
    if(motorsCreated == 0){
        //seteado en PWM0
        motorMemoryPool[motorsCreated].timer= MCPWM_TIMER_0;
        mcpwm_gpio_init(MCPWM_UNIT_0, MCPWM0A, motorMemoryPool[motorsCreated].pin_p); 
        mcpwm_gpio_init(MCPWM_UNIT_0, MCPWM0B, motorMemoryPool[motorsCreated].pin_n);
        printf("Configuring Initial Parameters of mcpwm0...\n");
        mcpwm_config_t pwm_config;
        pwm_config.frequency = 1000;    //frequency = 500Hz,
        pwm_config.cmpr_a = 0;    //duty cycle of PWMxA = 0
        pwm_config.cmpr_b = 0;    //duty cycle of PWMxb = 0
        pwm_config.counter_mode = MCPWM_UP_COUNTER;
        pwm_config.duty_mode = MCPWM_DUTY_MODE_0;
        mcpwm_init(MCPWM_UNIT_0, MCPWM_TIMER_0, &pwm_config);    //Configure PWM0A & PWM0B with above settings

    }else if(motorsCreated == 1){
        //seteado en PWM1
        motorMemoryPool[motorsCreated].timer= MCPWM_TIMER_1;
        mcpwm_gpio_init(MCPWM_UNIT_0, MCPWM1A, motorMemoryPool[motorsCreated].pin_p); 
        mcpwm_gpio_init(MCPWM_UNIT_0, MCPWM1B, motorMemoryPool[motorsCreated].pin_n);
        printf("Configuring Initial Parameters of mcpwm1...\n");
        mcpwm_config_t pwm_config;
        pwm_config.frequency = 1000;    //frequency = 500Hz,
        pwm_config.cmpr_a = 0;    //duty cycle of PWMxA = 0
        pwm_config.cmpr_b = 0;    //duty cycle of PWMxb = 0
        pwm_config.counter_mode = MCPWM_UP_COUNTER;
        pwm_config.duty_mode = MCPWM_DUTY_MODE_0;
        mcpwm_init(MCPWM_UNIT_0, MCPWM_TIMER_1, &pwm_config);    //Configure PWM1A & PWM1B with above settings

    }else{
        return 0;
         } 


    return &motorMemoryPool[motorsCreated++]; 
    //devuelve un puntero a motor con los pines configurados 
    //y setea el siguiente elemento del vector

    
}

/**
 * motor moves in forward direction, with duty cycle = duty %
 *///                                           
static void brushed_motor_forward(Motor_t config, float duty_cycle)
{
    mcpwm_set_signal_low(config->unit, config->timer, MCPWM_OPR_B);
    mcpwm_set_duty(config->unit, config->timer, MCPWM_OPR_A, duty_cycle);
    mcpwm_set_duty_type(config->unit, config->timer, MCPWM_OPR_A, MCPWM_DUTY_MODE_0); 
    //call this each time, if operator was previously in low/high state
}

/**
 * motor moves in backward direction, with duty cycle = duty %
 */
static void brushed_motor_backward(Motor_t config, float duty_cycle)
{
    mcpwm_set_signal_low(config->unit, config->timer, MCPWM_OPR_A);
    mcpwm_set_duty(config->unit, config->timer, MCPWM_OPR_B, duty_cycle);
    mcpwm_set_duty_type(config->unit, config->timer, MCPWM_OPR_B, MCPWM_DUTY_MODE_0);  
    //call this each time, if operator was previously in low/high state
}

/**
 * motor stop
 */
static void brushed_motor_stop(Motor_t config)
{
    mcpwm_set_signal_low(config->unit, config->timer, MCPWM_OPR_A);
    mcpwm_set_signal_low(config->unit, config->timer, MCPWM_OPR_B);
}