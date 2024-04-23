#include "linea_serie_drv.h"

static enum ESTADO_RECIBIR estado_recibir;

static EVENTO_T evFinRecibir, evFinMostrar, evContinuaMostrar;


static char buffer_recibir[3];
static char buffer_enviar[2000];
static uint32_t index_recibir = 0;
static uint32_t index_enviar = 0;


void interrupcion_serie(uint8_t caracter) {
	
	if (estado_recibir == WAIT) {
		if (caracter == '$') {
			estado_recibir = ALM;
			index_recibir = 0;
		}
		
	} else if (estado_recibir == ALM) {
		if (caracter == '!') {
			estado_recibir = WAIT;
			
			if (index_recibir == 3) {
				uint32_t auxData = atoi(buffer_recibir);
				callback_encolar(evFinRecibir, auxData);
			}
		} else if (index_recibir >= 3) {
			estado_recibir = LEN_ERROR;
			gpio_hal_escribir(GPIO_SERIE_ERROR, GPIO_SERIE_ERROR_BITS, 1);
		
		} else {
			buffer_recibir[index_recibir] = caracter;
			index_recibir++;
		}
		
	} else if (estado_recibir == LEN_ERROR) {
		if (caracter == '$') {
			index_recibir = 0;
			estado_recibir = ALM;
			gpio_hal_escribir(GPIO_SERIE_ERROR, GPIO_SERIE_ERROR_BITS, 0);
		}
	
	} else while(1);
}


void linea_serie_drv_iniciar(void (*funcion_callback)(), EVENTO_T finRecibir, 
	EVENTO_T finMostrar, EVENTO_T continuaMostrar) {
			
	estado_recibir = WAIT;
	serial0_hal_iniciar(&interrupcion_serie);
	index_enviar = 0;
	index_recibir = 0;
												
	evFinRecibir = finRecibir;
	evFinMostrar = finMostrar;
	evContinuaMostrar = continuaMostrar;
	callback_encolar = funcion_callback;
}


void linea_serie_drv_enviar_array(char salida[]) {
	int i;
	if (index_enviar != 0) return;

	for (i=0; salida[i] != '\0' && i<2000; i++) {
		buffer_enviar[i] = salida[i];
	}
	buffer_enviar[i] = salida[i];
	serial0_hal_escribir(buffer_enviar[index_enviar++]);
	callback_encolar(evContinuaMostrar, 0);
}

void linea_serie_drv_continuar_envio(void) {

	if (buffer_enviar[index_enviar] != '\0') {
		serial0_hal_escribir(buffer_enviar[index_enviar++]);
		callback_encolar(evContinuaMostrar, 0);
		
	} else {
		serial0_hal_escribir('\n');
		callback_encolar(evFinMostrar, 0);
		index_enviar = 0;
	}
}
