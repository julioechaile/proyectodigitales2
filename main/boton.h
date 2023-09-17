#ifndef LECTURA
#define LECTURA
//creacion de los estados de la maquina de estados

enum button_state      {
    button_state_up,
    button_state_fall,
    button_state_down,
    button_state_rise,
};


//creacion de la estructura del boton

typedef struct Boton * Boton_t;

//prototipo de la funcion update
enum button_state button_update (Boton_t boton_u);

//prototipo de funcion configuracion
Boton_t button_config (gpio_num_t pin);

#endif