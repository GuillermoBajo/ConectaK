#include <inttypes.h>

#include "evento.h"
#include "timer0_driver.h"

#ifndef ALARMAS_H
#define ALARMAS_H

#define ALARMAS_MAX 4

typedef struct {
	EVENTO_T 	ID_evento;
	uint64_t 	timestamp;
	uint32_t 	retardo;
	uint32_t 	auxData;
	uint8_t 	periodica;
} Alarma;

/**
 * @brief Inicializa el sistema de alarmas y configura la función de callback.
 * 
 * @param funcion_callback Puntero a la función de callback que se ejecutará al activarse una alarma.
 * @param ID_evento Identificador del evento asociado a las alarmas.
 */
void alarma_iniciar(void (*funcion_callback)(), EVENTO_T ID_evento);

/**
 * @brief Activa una alarma con el retardo especificado.
 * 
 * @param ID_evento Identificador del evento asociado a la alarma.
 * @param retardo Retardo en milisegundos antes de que se active la alarma.
 * @param auxData Datos adicionales asociados a la alarma.
 */
void alarma_activar(EVENTO_T ID_evento, uint32_t retardo, uint32_t auxData);

/**
 * @brief Función que se ejecuta periódicamente para tratar las alarmas.
 */
void alarma_tratar_evento(void);

/**
 * @brief Realiza pruebas de las funciones relacionadas con las alarmas.
 * 
 * @param FIFO_encolar Puntero a la función de encolado de eventos en la FIFO.
 * @param FIFO_iniciar Puntero a la función de inicialización de la FIFO.
 * @return 0 si las pruebas son exitosas, 1 en caso contrario.
 */
int alarmas_test(void (*FIFO_encolar)(EVENTO_T ID_evento, uint32_t auxData), 
							void (*FIFO_iniciar)(GPIO_HAL_PIN_T pin_overflow));

#endif
