#include "alarmas.h"

static void (*callback_encolar)(EVENTO_T ID_evento, uint32_t auxData);

static Alarma listaAlarmas[ALARMAS_MAX];
static uint8_t numAlarmas;

void alarma_iniciar(void (*funcion_callback)(), EVENTO_T ID_evento) {
	numAlarmas = 0;
	callback_encolar = funcion_callback;
	temporizador_drv_reloj(1, callback_encolar, ID_evento);
}


void extraer_alarma(EVENTO_T ID_evento) {
	int i;
	Alarma alarma_i, ultimaAlarma;

	for (i=0; i<numAlarmas; i++) {
		alarma_i = listaAlarmas[i];
		
		if (alarma_i.ID_evento == ID_evento) {
			ultimaAlarma = listaAlarmas[numAlarmas-1];
			listaAlarmas[i] = ultimaAlarma;
			numAlarmas--;
		}
	}
}

void alarma_activar(EVENTO_T ID_evento, uint32_t retardo, uint32_t auxData) {
	// extraemos la alarma con ID_evento (identificador unico)
	uint8_t periodica;
	uint64_t timestamp;
	Alarma nuevaAlarma;
	extraer_alarma(ID_evento);
	
	if (retardo == 0)
		return;
	
	if (numAlarmas >= ALARMAS_MAX) {
		callback_encolar(ALARMA_OVERFLOW, 0);
		return;
	}
	
	// hay retardo && alarmas disponibles -> encolamos
	periodica = (retardo & 0x80000000) != 0;
	retardo &= 0x7FFFFFFF;
	timestamp = temporizador_drv_leer();

	nuevaAlarma.ID_evento = ID_evento;
	nuevaAlarma.timestamp = timestamp + retardo*1000;
	nuevaAlarma.retardo = retardo;
	nuevaAlarma.auxData = auxData;
	nuevaAlarma.periodica = periodica;
	
	listaAlarmas[numAlarmas] = nuevaAlarma;
	numAlarmas++;
}

//-------------EVENTOS-------------//

// EVENTO ALARMA_TRATAR
void alarma_tratar_evento(void) {
	int i;
	uint64_t now = temporizador_drv_leer();
	Alarma alarma_i;
	
	for (i=0; i<numAlarmas; i++) {
		alarma_i = listaAlarmas[i];
		
		if (alarma_i.timestamp <= now) {
				callback_encolar(alarma_i.ID_evento, alarma_i.auxData);		
			if (alarma_i.ID_evento == ev_LATIDO) {
				alarma_i.ID_evento = ev_LATIDO;
			}
			if (alarma_i.periodica != 0) {
				// ponemos el bit de mas peso para indicar que es periodica
				alarma_i.retardo |= 0x80000000;
				alarma_activar(alarma_i.ID_evento, alarma_i.retardo, alarma_i.auxData);
			}
			else {
				extraer_alarma(alarma_i.ID_evento);
			}
		}
	}
}

//TEST ALARMAS
int alarmas_test(void (*FIFO_encolar)(EVENTO_T ID_evento, uint32_t auxData), 
							void (*FIFO_iniciar)(GPIO_HAL_PIN_T pin_overflow)){
								
    uint8_t 	fifoExt;
    uint32_t 	retardoPeriodico, auxData;
    EVENTO_T 	ID_evento;
    uint8_t sigue = 1;
    
    temporizador_drv_iniciar();
    temporizador_drv_empezar();
	
    FIFO_iniciar(GPIO_OVERFLOW);
	
    // Inicializa las alarmas
    alarma_iniciar(FIFO_encolar, ALARMA_TRATAR);

    // 55ms y se activa el ultimo bit
    retardoPeriodico = 30 | 0x80000000;
	
    // Activa algunas alarmas de prueba
    alarma_activar(VOID, 20, 3); // Alarma con ID 1, retardo de 2000
    alarma_activar(TIMER0, retardoPeriodico, 0); // Alarma con ID 2, retardo de 3000

    // primera alarma
    while (temporizador_drv_leer() < 22 && sigue) {
        alarma_tratar_evento();
        
        fifoExt = FIFO_extraer(&ID_evento, &auxData);
        if (fifoExt != 0) {	
            if (ID_evento != 0)	return 1;
            if (auxData != 3) return 1;
        } else {
            sigue = 0;
        }
    }
		
    // segunda alarma temp
    sigue = 1;
    while (temporizador_drv_leer() < 32 && sigue) {
        alarma_tratar_evento();
        
        fifoExt = FIFO_extraer(&ID_evento, &auxData);
        if (fifoExt != 0) {	// no ha desencolado nada
            if (ID_evento != 1)	return 1;
            if (auxData != 0) return 1;
        } else {
            sigue = 0;
        }
    }
    
    // segunda alarma, ha encolado con temporizador
    sigue = 1;
    while (temporizador_drv_leer() < 64) {
        alarma_tratar_evento();
        
        fifoExt = FIFO_extraer(&ID_evento, &auxData);
        if (fifoExt != 0) {	// no ha desencolado nada
            if (ID_evento != 1)	return 1;
            if (auxData != 0) return 1;
        } else {
            sigue = 0;
        }
        
        alarma_activar(TIMER0, 0, 0); // Alarma con ID 2, retardo de 3000
    }
    
    // segunda alarma, la ha quitado
    while (temporizador_drv_leer() < 94) {
        alarma_tratar_evento();
        
        fifoExt = FIFO_extraer(&ID_evento, &auxData);
        if (fifoExt != 0) return 1;	// no ha desencolado nada
    }
    
    alarma_activar(VOID, 20, 3);
    alarma_activar(TIMER0, 20, 3);
    alarma_activar(TIMER1, 20, 3);
    alarma_activar(ALARMA_OVERFLOW, 20, 3);
    alarma_activar(ALARMA_TRATAR, 20, 3);
    
    fifoExt = FIFO_extraer(&ID_evento, &auxData);
    if (fifoExt != 1) return 1;	// no ha desencolado nada
    if (ID_evento != ALARMA_OVERFLOW) return 1;

    return 0;
}
