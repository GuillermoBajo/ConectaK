#include <inttypes.h>
#include <stddef.h>

#ifndef TIMER0_HAL_H
#define TIMER0_HAL_H

static const float temporizador_hal_tick2us = 0.067;
static void (*timer1_routine)(void);

/**
 * @brief Prueba el correcto funcionamiento del temporizador.
 *
 * Esta función realiza una prueba para verificar el correcto funcionamiento del temporizador.
 * Inicia el temporizador, lo pone en marcha, lee el valor del temporizador y lo compara con un valor esperado.
 * Finalmente, detiene el temporizador y verifica que el valor leído sea igual al valor esperado.
 *
 * @return 0 si la prueba es exitosa, 1 en caso contrario.
 */
int temporizador_hal_test(void);

/**
 * @brief Inicializa el temporizador 0.
 *
 * Esta función inicializa el temporizador 0, reiniciando el contador y configurando los registros necesarios.
 */
void temporizador_hal_iniciar(void);

/**
 * @brief Pone en marcha el temporizador 0.
 *
 * Esta función pone en marcha el temporizador 0, habilitando su contador y activando la interrupción periódica.
 */
void temporizador_hal_empezar(void);

/**
 * @brief Lee el valor actual del temporizador 0.
 *
 * Esta función lee el valor actual del temporizador 0 y lo devuelve en ticks.
 *
 * @return El valor actual del temporizador 0 en ticks.
 */
uint64_t temporizador_hal_leer(void);

/**
 * @brief Detiene el temporizador 0 y devuelve el tiempo transcurrido.
 *
 * Esta función detiene el temporizador 0 y devuelve el tiempo transcurrido desde la última vez que se inició el temporizador.
 *
 * @return El tiempo transcurrido en ticks desde la última vez que se inició el temporizador.
 */
uint64_t temporizador_hal_parar(void);

/**
 * @brief Configura el reloj del temporizador 1 para llamar a una función de callback periódicamente.
 *
 * Esta función configura el reloj del temporizador 1 para que llame a la función de callback especificada cada periodo.
 * El periodo se indica en milisegundos. Si el periodo es cero, se detiene el temporizador.
 *
 * @param periodo El periodo en milisegundos.
 * @param funcion_callback La función de callback a llamar periódicamente.
 */
void temporizador_hal_reloj(uint32_t periodo, void (*funcion_callback)());

#endif
