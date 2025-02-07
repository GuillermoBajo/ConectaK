#include <LPC210X.H>    // LPC21XX Peripheral Registers
#include "system_calls.h"

#ifndef WATCHDOG_H
#define WATCHDOG_H

/**
 * @brief Inicializa el watchdog timer con el tiempo de cuenta especificado en segundos.
 * 
 * @param sec Tiempo de cuenta en segundos.
 */
void WD_hal_iniciar(int sec);

/**
 * @brief "Alimenta" el watchdog timer para evitar el reset.
 * 
 * Esta función debe ser llamada periódicamente para evitar que el watchdog timer
 * genere un reset. Deshabilita las interrupciones antes de alimentar el watchdog
 * y las habilita nuevamente después.
 */
void WD_hal_feed(void);

/**
 * @brief Función de prueba con un bucle infinito para comprobar el watchdog.
 * 
 * Esta función inicializa el watchdog timer con un tiempo de cuenta de 5 segundos
 * y luego llama a la función WD_hal_feed() para "alimentar" el watchdog. A continuación,
 * entra en un bucle infinito para simular una ejecución continua del programa.
 */
void WD_hal_test(void);

#endif
