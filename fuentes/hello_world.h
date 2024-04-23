#include <inttypes.h>

#include "timer0_driver.h"
#include "alarmas.h"
#include "evento.h"

#ifndef HELLO_H
#define HELLO_H

static void (*callback_encolar)(EVENTO_T ID_evento, uint32_t auxData);

/**
 * @brief Inicializa el módulo "Hello World".
 *
 * Esta función se encarga de inicializar el módulo "Hello World". Configura los pines de salida
 * necesarios, activa una alarma y guarda el evento de encolar y la función de callback proporcionados.
 *
 * @param funcion_callback Puntero a la función de callback que se llamará al encolar un evento.
 * @param ID_evento_alarma Identificador del evento de alarma.
 * @param ID_evento_encolar Identificador del evento de encolar.
 */
void hello_world_iniciar(void (*funcion_callback)(), EVENTO_T ID_evento_alarma, EVENTO_T ID_evento_encolar);


/**
 * @brief Realiza una operación de "tick-tack" en el módulo "Hello World".
 *
 * Representa en binario el valor del contador temporal en la salida.
 */
void hello_world_tick_tack(void);


/**
 * @brief Trata un evento en el módulo "Hello World".
 *
 * Incrementa el contador temporal y llama a la función de callback proporcionada
 * pasando el evento de encolar y el contador temporal como argumentos.
 */
void hello_world_tratar_evento(void);

#endif
