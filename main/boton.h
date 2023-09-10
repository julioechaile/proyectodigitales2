#ifndef LECTURA
#define LECTURA
//creacion de los estados de la maquina de estados

enum button_state      {
    button_state_up,
    button_state_fall,
    button_state_down,
    button_state_rise,
};


//creacion de la estructura

struct button          
{
    enum button_state state;
    gpio_num_t pin;     //direccion del pin a leer
};

//prototipo de la funcion update
void button_update (struct button *boton_u);

//prototipo de funcion configuracion
void button_config (struct button *boton_u, gpio_num_t pin);

#endif