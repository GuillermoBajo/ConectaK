#include <inttypes.h>
#include <LPC210X.H>    // LPC21XX Peripheral Registers

#include "gpio.h"
#include "system_calls.h"
#include "evento.h"

#ifndef FIFO_H
#define FIFO_H


typedef struct {
    EventoCola eventos[MAX_EVENTOS];
    uint8_t inicio;
    uint8_t fin;
    GPIO_HAL_PIN_T pin_overflow;
    uint8_t overflow;
    uint32_t estadisticas[NUM_TIPO_EVENTOS];
} FIFO;


// Inicialización de la cola. Se le pasa como parámetro el pin del 
// GPIO utilizado para marcar errores.
void FIFO_iniciar(GPIO_HAL_PIN_T pin_overflow);

// Esta función guardará en la cola el evento. El campo ID_evento, 
// que permita identificar el evento (p.e. qué interrupción ha saltado) 
// y el campo auxData en caso de que el evento necesite pasar información extra.
void  FIFO_encolar(EVENTO_T ID_evento, uint32_t auxData);

// Si hay eventos sin procesar, devuelve un valor distinto de cero y 
// el evento más antiguo sin procesar por referencia. Cero indicará que la 
// cola está vacía y no se ha devuelto ningún evento.
uint8_t FIFO_extraer(EVENTO_T *ID_evento, uint32_t* auxData);

// Dado un identificador de evento nos devuelve el número total de veces 
// que ese evento se ha encolado. El evento VOID nos devolverá el total de 
// eventos encolados desde el inicio.
uint32_t FIFO_estadisticas(EVENTO_T ID_evento);

// Se pone el pin de overflow a 1 y se para la ejecucion (bucle infinito)
void FIFO_overflow(void);

int FIFO_test(int overflow);

#endif
