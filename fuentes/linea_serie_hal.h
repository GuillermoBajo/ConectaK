#include <LPC210X.H>    // LPC21XX Peripheral Registers
#include <stddef.h>
#include "system_calls.h"

#ifndef LINEA_SERIE_HAL_H
#define LINEA_SERIE_HAL_H

/**
 * @brief Inicializa la interfaz serie 0.
 * @param interrupcion_serie0_par Puntero a la función de interrupción para la serie 0.
 */
void serial0_hal_iniciar(void (*interrupcion_eint_par)(uint8_t));

/**
 * @brief Verifica si se ha recibido un carácter en la interfaz serie 0.
 * @return 1 si se ha recibido un carácter, 0 en caso contrario.
 */
uint32_t serial0_hal_char_recibido(void);

/**
 * @brief Escribe un carácter en la interfaz serie 0.
 * @param caracter El carácter a escribir.
 */
void serial0_hal_escribir(char caracter);

#endif
