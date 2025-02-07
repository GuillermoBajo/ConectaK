#include "gpio.h"

#ifndef VISUALIZAR_H
#define VISUALIZAR_H

/**
 * @brief Inicializa la salida.
 * 
 * Configura los pines correspondientes como salidas.
 */
void visualizar_iniciar(void);

/**
 * @brief Muestra una cuenta en la salida.
 * 
 * @param cuenta El valor de la cuenta a mostrar.
 */
void visualizar_cuenta(int cuenta);

/**
 * @brief Muestra el valor de la variable en la salida.
 * 
 * @param temp El valor de la variable a mostrar.
 */
void visualizar_hello_world(int temp);

#endif
