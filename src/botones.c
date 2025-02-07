#include "botones.h"

static enum ESTADO estado_boton_1, estado_boton_2;

static void (*callback_encolar)(EVENTO_T ID_evento, uint32_t auxData);
static EVENTO_T eventoEncolar;

static void (*callback_temp)(EVENTO_T ID_evento, uint32_t retardo, uint32_t auxData);
static EVENTO_T eventoTemp;


void interrupcion_eint(uint8_t num_boton) {
	// si el boton no esta pulsado y se interrumpe, encolamos evento
	// pulsacion y establecemos estado del boton como pulsado
	if (num_boton == 1) {
		if (estado_boton_1 == NO_PULSADO) {
			callback_encolar(eventoEncolar, num_boton);
		}
	} else if (num_boton == 2) {
		if (estado_boton_2 == NO_PULSADO) {
			callback_encolar(eventoEncolar, num_boton);
		}
	}
}

void botones_iniciar(void (*callback_cola)(), EVENTO_T ev_pulsacion,
								void (*callback_temporizador)(), EVENTO_T ev_temporizador) {
	// establecemos estado de los botones
	estado_boton_1 = NO_PULSADO;
	estado_boton_2 = NO_PULSADO;
	// inicializamos los botones y pasamos la rutina que se ejecuta en la interrupcion
	eint1_iniciar(&interrupcion_eint);
	eint2_iniciar(&interrupcion_eint);
	
	eventoEncolar = ev_pulsacion;
	callback_encolar = callback_cola;
									
	eventoTemp = ev_temporizador;
	callback_temp = callback_temporizador;
}


//-------------EVENTOS-------------//

// EVENTO COMPROBAR_BOTON
int boton_esta_pulsado(void) {
	// actualizamos el estado de los botones
	if (eint1_read_button() == 0) {
		estado_boton_1 = NO_PULSADO;
		eint1_enable();
	
	} 
	if (eint2_read_button() == 0) {
		estado_boton_2 = NO_PULSADO;
		eint2_enable();
	}
	
	if (estado_boton_1 == NO_PULSADO && estado_boton_2 == NO_PULSADO) {
		// si no esta pulsado ningun boton desactivamos la comprobaci�n
		callback_temp(eventoTemp, 0, 0);
		return 0;
	}
	
	return 1;
}

// EVENTO PULSACION_BOTON
void handle_pulsacion_boton(uint8_t num_boton) {	
	callback_temp(eventoTemp, 100 | 0x80000000, 0);
	
	// completar alarma usuario ausente
	if (num_boton == 1)
		estado_boton_1 = PULSADO;
	else if (num_boton == 2)
		estado_boton_2 = PULSADO;
}

// Función para probar el módulo de botones
int botones_test(void) {
	return int_externas_hal_test();
}

