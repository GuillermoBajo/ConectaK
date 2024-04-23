#include "system_calls.h"
#include "timer0_driver.h"

uint32_t system_calls_test(void){
	uint32_t tiempo1;
	uint32_t tiempo2;
	uint32_t bitIRQ;
	
	temporizador_drv_iniciar();
	temporizador_drv_empezar();
	
	enable_irq();
	
	tiempo1 = temporizador_drv_leer();
	tiempo2 = clock_getus();
	tiempo1 = tiempo1 + 3;
	if(tiempo1 != tiempo2) return 1;
	
	bitIRQ = read_IRQ_bit();

	if(bitIRQ != 0) return 1;
	
	disable_irq();
	
	bitIRQ = read_IRQ_bit();	
	
	if(bitIRQ != 1) return 1;
	
	return 0;
}

