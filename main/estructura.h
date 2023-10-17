/*como haria una estructuracion de datos para que el programa sea reutilizable en todas sus partes?

estructura de datos:

 logica de negocio: 
    Se inicializa:
        llama a inicializar sensores
            recibe tantos punteros a sensores como haya pedido
        llama a innicializar eje
            recibe un puntero al eje
    Lee estado de sensores (leer_sensor)y se pone en estos estados:
    	-robot{
		detenido
		avanzar
		retroceder
		izquierda
		derecha
		}
    llama al eje y le pasa el estado a setear (setear_eje)

    repetir

*/
#ifndef estructura_H
#define estructura_H 
#include <stdint.h>
//#include "driver/gpio.h"
#include "boton.h"
#include "eje.h"
//creacion de la maquina de estados del robot
/*enum estado
{
    estado_avanzar,
    estado_derecha,
    estado_izquierda,
    estado_reversa,
    estado_detenido,
};*/

struct robot
{
    enum estado status;
    Boton_t sensor_derecha;
    Boton_t sensor_izquierda;
    Boton_t sensor_retroceso;
};
typedef struct robot robot_t;
#endif