#include "fifo.h"

static FIFO cola;

// Inicialización de la cola. Se le pasa como parámetro el pin del 
// GPIO utilizado para marcar errores.
void FIFO_iniciar(GPIO_HAL_PIN_T pin_overflow) {
	  int i;
		gpio_hal_sentido(GPIO_OVERFLOW, GPIO_OVERFLOW_BITS, GPIO_HAL_PIN_DIR_OUTPUT);
    cola.pin_overflow = pin_overflow;
    cola.inicio = 0;
    cola.fin = 0;
    cola.overflow = 0;
    for (i = 0; i < NUM_TIPO_EVENTOS; i++) {
        cola.estadisticas[i] = 0;
    }
}

// Esta función guardará en la cola el evento. El campo ID_evento, 
// que permita identificar el evento (p.e. qué interrupción ha saltado) 
// y el campo auxData en caso de que el evento necesite pasar información extra.
void FIFO_encolar(EVENTO_T ID_evento, uint32_t auxData) {  
		uint8_t state_irq_bit;

		state_irq_bit = read_IRQ_bit();
		if (state_irq_bit == 0) {
			disable_irq();
		}
    cola.eventos[cola.fin].ID_evento = ID_evento;
    cola.eventos[cola.fin].auxData = auxData;
    cola.estadisticas[ID_evento]++; // incrementamos el contador de eventos
    cola.fin++;

    if (cola.fin == MAX_EVENTOS) { // si fin llega al final, lo ponemos al principio
        cola.fin = 0;
    }
    if (cola.fin == cola.inicio) {  // si encolar uno mas nos lleva a la posicion de inicio, overflow
        FIFO_overflow();
    }
		if (state_irq_bit == 0){
			enable_irq();
		}
}

// Si hay eventos sin procesar, devuelve un valor distinto de cero y 
// el evento más antiguo sin procesar por referencia. Cero indicará que la 
// cola está vacía y no se ha devuelto ningún evento.
uint8_t FIFO_extraer(EVENTO_T *ID_evento, uint32_t* auxData) {
	uint8_t state_irq_bit;
	if (cola.inicio == cola.fin) {    // si inicio y fin son iguales, no hay eventos  
		return 0;
	}
	else {
		state_irq_bit = read_IRQ_bit();
		if (state_irq_bit == 0)
			disable_irq();
		
		*ID_evento = cola.eventos[cola.inicio].ID_evento;
		*auxData = cola.eventos[cola.inicio].auxData;
		cola.inicio++;
		if (cola.inicio == MAX_EVENTOS) {
			cola.inicio = 0;
		}
		if (state_irq_bit == 0)
			enable_irq();
	}	
	return 1;
}

// Dado un identificador de evento nos devuelve el número total de veces 
// que ese evento se ha encolado. El evento VOID nos devolverá el total de 
// eventos encolados desde el inicio.
uint32_t FIFO_estadisticas(EVENTO_T ID_evento) {
    if (ID_evento == VOID) {		
			int i;
			uint32_t total = 0;
			for (i = 1; i < NUM_TIPO_EVENTOS; i++) {
				total += cola.estadisticas[i];
			}
			return total;
    }
    return cola.estadisticas[ID_evento];
}

void FIFO_overflow(void) {
	cola.overflow = 1;
	gpio_hal_escribir(cola.pin_overflow, 0x01, 1);
	while(1);
}

// FIFO TEST
int FIFO_test(int overflow) {
	uint32_t i, test_encolar;
	uint8_t	test_extraer;

	EventoCola EC;

  FIFO_iniciar(31);
		
	// TESTEA QUE DETECTA COLA VACIA
	test_extraer = FIFO_extraer(&EC.ID_evento, &EC.auxData);
	if (test_extraer != 0) return 1;
	
	// TESTEA QUE ENCOLA Y ESTADISTICAS SON ADECUADAS
	for (i = 0; i < 4; i++) {
		FIFO_encolar(TIMER1, 3);
	}
	FIFO_encolar(TIMER0, 4);

	test_encolar = FIFO_estadisticas(VOID);
	if (test_encolar != 5) return 2;
	test_encolar = FIFO_estadisticas(TIMER1);
	if (test_encolar != 4) return 2;
	
	// TESTEA QUE EXTRAE EL EVENTO
	test_extraer = FIFO_extraer(&EC.ID_evento, &EC.auxData);
	if (EC.ID_evento != TIMER1 || EC.auxData != 3) return 3;
	
	// TEST OVERFLOW -> DESCOMENTAR PARA TEST
	if (overflow) {
		for (i = 0; i < MAX_EVENTOS-4; i++){
			FIFO_encolar(VOID, 5);
		}
	}
	
	return 0;
}
