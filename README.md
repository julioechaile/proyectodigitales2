| Supported Targets | ESP32 | ESP32-C2 | ESP32-C3 | ESP32-S2 | ESP32-S3 |
| ----------------- | ----- | -------- | -------- | -------- | -------- |
Universidad de Moron                                                                   Tecnicas digitales II
                                         Ingenieria Electronica

Alumno: Julio Eduardo Chaile

Proyecto "Robot Barredor"

Objetivo del Proyecto:
El objetivo del proyecto es aplicar los conceptos aprendidos en la catedra, en sentido de la aplicacion de FreeRTOS en un sistema embebido.
El sistema elegido es el ESP32.

Resumen del Proyecto:
Se busca implementar un robot basico que posea los siguientes atributos:
-Logica de nogocio.
-Diferentes API, que realizarán las tareas secundarias.

Caracteristicas del robot:
El robot posee los siguientes componentes:

-ESP32, donde estará cargado el software
-3 sensores de colision, con los que el robot sensará si a tomado contacto fisico, a saber:
        -A su derecha.
        -A su izquierda.
        -Si ha chocado de frente.
-2 Motores de corriente continua, alimentados con un puente H doble, como se describirá mas adelante.
-1 toma de sensor desde la batería, atenuado a la mitad para no pasar el límite que puede medir el ADC.
-Bateria
-Circuito cargador y Step-UP para la batería.

Funcionamiento:
El robot comenzará con una fase de inicializacion, en la que se incializan los sensores, luego se inicializará la entrada analógica para medir tension.
finalmente se inicializará la salida PWM para el funcionamineto de los motores de CC, que lo impulsan.

Una vez finalizada la inicializacion y corroborando que posee el nivel de bateria necesario, el robot procede a actualizar su estado, segun el estado de sus sensores.

tabla explicativa:

Sensor izq | sensor Der | sensor Ret | Conducta
    0           0           0           marcha adelante
    1           0           0           giro derecha
    0           1           0           giro izquierda
    0           0           1           retroceso

Los demas estados posibles son ignorados en la lógica por cuestiones de simplicidad, pero de accionarse mas de un sensor, la maquina reaccionará en el orden de comportamientos como está presentado arriba.

Como utilizar el proyecto:

Dentro de la carpeta de proyecto, abrir terminal y inicializar el toolchain con

                        get_idf

Una vez cargado el sistema ejecutar

                        idf-py build

Esto comenzará la compilacion del proyecto, una vez finalizado, ejecutar

                        idf.py -p /dev/ttyUSB0 flash monitor

Este comando subirá el software al ESP32 e inicializará la aplicación de monitoreo, donde podremos ver la inicializacion y las diferentes conductas que tomará la maquina según el estado de sus sensores.

Para finalizar, presionar CTRL + ]




