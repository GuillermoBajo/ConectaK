#include "hello_world.h"

static int temp = 0;
static EVENTO_T eventoEncolar;

void hello_world_iniciar(void (*funcion_callback)(), EVENTO_T ID_evento_alarma, EVENTO_T ID_evento_encolar) {	
	// primer bit(0), num_bits, sentido
	gpio_hal_sentido(GPIO_HELLO_WORLD, GPIO_HELLO_WORLD_BITS, GPIO_HAL_PIN_DIR_OUTPUT);
	temp = 0;
	alarma_activar(ID_evento_alarma, 10 | 0x80000000, 0);
	eventoEncolar = ID_evento_encolar;
	callback_encolar = funcion_callback;
}


void hello_world_tick_tack(void) {
	// enciende en temp impares, apaga en pares
	temp++;
	gpio_hal_escribir(GPIO_HELLO_WORLD, GPIO_HELLO_WORLD_BITS, temp%(1 << 8));
}

void hello_world_tratar_evento(void) {
	temp++;
	callback_encolar(eventoEncolar, temp);
}
