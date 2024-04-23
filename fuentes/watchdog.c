#include "watchdog.h"

// Funci�n para inicializar el watchdog timer
void WD_hal_iniciar(int sec) {
    // Configurar el tiempo de cuenta en segundos
    WDTC = sec * 15000000 / 4; 	//ESTO ES ASI -> en video dice algo asi???????????????????????????????????????????????????????????

    // Configurar el watchdog para generar un reset y habilitarlo
    WDMOD = 3;
		
}

// Funci�n para "alimentar" el watchdog timer y evitar el reset
void WD_hal_feed() {
	uint8_t state_irq_bit	= read_IRQ_bit();
	disable_fiq();
	if (state_irq_bit == 0)
		disable_irq();
		
	WDFEED = 0xAA;
	WDFEED = 0x55;
	
	if (state_irq_bit == 0)
		enable_irq();
}

// Funci�n de prueba con un bucle infinito para comprobar el watchdog
void WD_hal_test() {   
    WD_hal_iniciar(5);
    WD_hal_feed();
    while (1) {}
}
