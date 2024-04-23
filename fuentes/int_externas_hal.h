#include <LPC210X.H>
#include <stddef.h>
#include <inttypes.h>


#ifndef INT_EXTERNAS_HAL_H
#define INT_EXTERNAS_HAL_H

//EINT1
/**
 * @brief Inicializa la interrupción EINT1.
 *
 * Esta función configura los registros necesarios para habilitar la interrupción EINT1.
 *
 * @param interrupcion_eint_par Puntero a la función de interrupción EINT1.
 */
void eint1_iniciar(void (*interrupcion_eint_par)(uint8_t));

/**
 * @brief Lee el estado del botón asociado a EINT1.
 *
 * Esta función lee el estado del botón asociado a EINT1 y devuelve su valor.
 *
 * @return Estado del botón asociado a EINT1 (0 o 1).
 */
unsigned int eint1_read_button(void);

/**
 * @brief Habilita la interrupción EINT1.
 *
 * Esta función habilita la interrupción EINT1.
 */
void eint1_enable(void);

//EINT2
/**
 * @brief Inicializa la interrupción EINT2.
 *
 * Esta función configura los registros necesarios para habilitar la interrupción EINT2.
 *
 * @param interrupcion_eint_par Puntero a la función de interrupción EINT2.
 */
void eint2_iniciar(void (*interrupcion_eint_par)(uint8_t));

/**
 * @brief Lee el estado del botón asociado a EINT2.
 *
 * Esta función lee el estado del botón asociado a EINT2 y devuelve su valor.
 *
 * @return Estado del botón asociado a EINT2 (0 o 1).
 */
unsigned int eint2_read_button(void);

/**
 * @brief Habilita la interrupción EINT2.
 *
 * Esta función habilita la interrupción EINT2.
 */
void eint2_enable(void);

/**
 * @brief Función de prueba para el módulo de interrupciones externas.
 *
 * Esta función realiza una prueba de las funciones de manejo de interrupciones externas EINT1 y EINT2.
 *
 * @return 0 si la prueba se completó correctamente.
 */
int int_externas_hal_test(void);
	
#endif
