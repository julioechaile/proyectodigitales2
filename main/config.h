#ifndef CONFIG_H
#define CONFIG_H

//pines de salida de motores
#define PIN_P_MOTA 17 //GPIO_NUM_17
#define PIN_N_MOTA 5 //GPIO_NUM_5
#define PIN_P_MOTB 18  //GPIO_NUM_18
#define PIN_N_MOTB 19 //GPIO_NUM_19

//pines de los sensores
#define motores_usados 2
#define Sensores_maximos 3
#define sensor_izq GPIO_NUM_15
#define sensor_der GPIO_NUM_2
#define sensor_ret GPIO_NUM_0
#define voltage_pin GPIO_NUM_32

#endif // CONFIG_H