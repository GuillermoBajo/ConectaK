#include "int_externas_hal.h"

static void (*interrupcion_eint1)(uint8_t);
static void (*interrupcion_eint2)(uint8_t);


//EINT1
void eint1_ISR(void) __irq {
	VICIntEnClr = 0x00008000; 
	if (interrupcion_eint1 != NULL)
		interrupcion_eint1(1);
	
	EXTINT = EXTINT | 2;
	VICVectAddr = 0;
}


void eint1_iniciar(void (*interrupcion_eint_par)(uint8_t)) {
	EXTINT =  EXTINT | 2;
	VICVectAddr2 = (unsigned long)eint1_ISR;
	PINSEL0 		= PINSEL0 & 0xCFFFFFFF;
	PINSEL0 		= PINSEL0 | 0x20000000;
	VICVectCntl2 = 0x20 | 15;
	VICIntEnable = VICIntEnable | 0x00008000;

	interrupcion_eint1 = interrupcion_eint_par;
}


unsigned int eint1_read_button(void){
	EXTINT = EXTINT | 2;
	return (EXTINT & (1 << 1)) >> 1;
};


void eint1_enable(void){
	EXTINT =  EXTINT | 2;
	VICIntEnable = VICIntEnable | 0x00008000;
}


//EINT2
void eint2_ISR(void) __irq {
	VICIntEnClr = 0x00010000; 
	if (interrupcion_eint2 != NULL)
		interrupcion_eint2(2);

	VICVectAddr = 0;
	EXTINT = EXTINT | 4;
}


void eint2_iniciar(void (*interrupcion_eint_par)(uint8_t)) {
	EXTINT =  EXTINT | 4;
	VICVectAddr3 = (unsigned long)eint2_ISR;
	PINSEL0 		= PINSEL0 & 0x3FFFFFFF;	
	PINSEL0 		= PINSEL0 | 0x80000000;	
	VICVectCntl3 = 0x20 | 16;                   
 	VICIntEnable = VICIntEnable | 0x00010000;
	
	interrupcion_eint2 = interrupcion_eint_par;
}


unsigned int eint2_read_button(void){
	EXTINT =  EXTINT | 4;
	return (EXTINT & (1 << 2)) >> 2;
};


void eint2_enable(void){
	EXTINT =  EXTINT | 4;
	VICIntEnable = VICIntEnable | 0x00010000;
}


int int_externas_hal_test(void) {
	eint1_iniciar(NULL);
	eint2_iniciar(NULL);

	while (eint1_read_button() != 1) {}
	while (eint2_read_button() != 1) {}

	return 0;
}
