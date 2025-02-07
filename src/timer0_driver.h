#include "timer0_hal.h"
#include "fifo.h"

#ifndef TIMER0_DRIVER_H
#define TIMER0_DRIVER_H

static void (*encolar_evento)(EVENTO_T, uint32_t);
static EVENTO_T evento;

/**
 * @brief Inicializa el temporizador.
 *
 * Esta función inicializa el hardware asociado al temporizador. No empieza a contar.
 */
void temporizador_drv_iniciar(void);

/**
 * @brief Empieza el temporizador.
 *
 * Esta función empieza el temporizador 0.
 */
void temporizador_drv_empezar(void);

/**
 * @brief Lee el valor del temporizador.
 *
 * @return El valor del temporizador en microsegundos.
 */
uint64_t temporizador_drv_leer(void);;

/**
 * @brief Para el temporizador y devuelve el valor actual.
 *
 * @return El valor del temporizador en microsegundos.
 */
uint64_t temporizador_drv_parar(void);

/**
 * @brief Programa un reloj periódico.
 *
 * Esta función programa el reloj para que encole un evento periódicamente en la cola del planificador.
 *
 * @param periodo El periodo del reloj en milisegundos.
 * @param funcion_encolar_evento Puntero a la función que encola el evento.
 * @param ID_evento El ID del evento a encolar.
 */
void temporizador_drv_reloj(uint32_t periodo, void (*funcion_encolar_evento)(), EVENTO_T ID_evento);

/**
 * @brief Prueba del controlador del temporizador.
 *
 * Esta función prueba el controlador del temporizador.
 *
 * @return El resultado de la prueba.
 */
int timer0_driver_test(void);

/**
 * @brief Función SWI para leer el temporizador.
 *
 * @return El valor del temporizador en microsegundos.
 */
uint32_t __swi(0) clock_getus(void);

#endif
