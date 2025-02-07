#include "linea_serie_hal.h"

static void (*interrupcion_serie0)(uint8_t);


void serie0_ISR(void) __irq {
	if (interrupcion_serie0 != NULL && serial0_hal_char_recibido())
		interrupcion_serie0(U0RBR);
	
	VICVectAddr = 0;							// Reconoce interrupcion escribiendo ahi un 0
}

void serial0_hal_iniciar(void (*interrupcion_serie0_par)(uint8_t))  {               /* Initialize Serial Interface       */
  PINSEL0 |= 0x5;                  /* Enable RxD1 and TxD1              */
  U0LCR = 0x83;                          /* 8 bits, no Parity, 1 Stop bit     */
  U0DLL = 97;                            /* 9600 Baud Rate @ 15MHz VPB Clock  */
  U0LCR = 0x03;                          /* DLAB = 0                          */
	U0IER = 3;
	
	interrupcion_serie0 = interrupcion_serie0_par;
	VICVectCntl4 = 0x20 | 6;
	VICVectAddr4 = (unsigned long)serie0_ISR;
	VICIntEnable = VICIntEnable |  0x00000040;
}

uint32_t serial0_hal_char_recibido(void) {
	// quitamos el bit 0, si solo esta el bit 2, char recibido
	// 010 2a - Receive Data Available (RDA).
	return (U0IIR & 0xE) == 4; 
}

void serial0_hal_escribir(char caracter) {
	//U0IER |= 0x2;
	while (!(U0LSR & 0x20));
	U0THR = caracter;
}
