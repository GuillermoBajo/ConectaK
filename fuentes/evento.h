#include <inttypes.h>

#ifndef EVENTO_H
#define EVENTO_H

#define MAX_EVENTOS 32
#define NUM_TIPO_EVENTOS 14

/**
 * @brief Enumeración que representa los diferentes tipos de eventos.
 * 
 * Los valores de esta enumeración representan los distintos tipos de eventos que pueden ocurrir en el sistema.
 * Cada valor tiene un significado específico y se utiliza para identificar el tipo de evento que se está manejando.
 */
typedef enum {
	VOID = 0,
	TIMER0 = 1,
	TIMER1 = 2,
	ALARMA_OVERFLOW = 3,
	ALARMA_TRATAR = 4,
	PULSACION_BOTON = 5,
	COMPROBAR_BOTON = 6,
	USUARIO_AUSENTE = 7,
	ev_VISUALIZAR_CUENTA = 8,
	ev_LATIDO = 9,
	ev_VISUALIZAR_HELLO = 10,
	ev_TX_SERIE = 11,
	ev_RX_SERIE = 12,
	CONTINUA_ENVIO = 13,
	JUGADA_CONFIRM_TEMP = 14,
	
} EVENTO_T;

typedef struct {
    EVENTO_T ID_evento;
    uint32_t auxData;
} EventoCola;

#endif
