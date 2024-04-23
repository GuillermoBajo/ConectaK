#include "timer0_driver.h"

void temporizador_drv_iniciar(void){
	temporizador_hal_iniciar();
}

void temporizador_drv_empezar(void){
	temporizador_hal_empezar();
}

uint64_t temporizador_drv_leer(void){
	return temporizador_hal_leer() * temporizador_hal_tick2us;
}

uint64_t temporizador_drv_parar(void){
	return temporizador_hal_parar() * temporizador_hal_tick2us;
}

void callback_reloj(void) {
	encolar_evento(evento, 0);
}

// función que programa el reloj para que encole un evento periódicamente en la 
// cola del planificador. El periodo se indica en ms.
void temporizador_drv_reloj(uint32_t periodo, void (*funcion_encolar_evento)(), EVENTO_T ID_evento) {
	void (*callback_reloj_ptr)(void) = callback_reloj;
	encolar_evento = funcion_encolar_evento;
	evento = ID_evento;
	
	temporizador_hal_reloj(periodo, callback_reloj_ptr);
}

int timer0_driver_test(void) {
	temporizador_drv_iniciar();
	temporizador_drv_empezar();
	  
	return temporizador_hal_test();
}


uint32_t __SWI_0 (void){
	return temporizador_drv_leer();
}
