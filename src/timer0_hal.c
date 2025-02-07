#include <LPC210X.H>
#include "timer0_hal.h"
static volatile unsigned int timer0_count = 0;


// Interrupt service routine
void timer0_ISR (void) __irq{    // Generate Interrupt 
	timer0_count++;
	T0IR = 1;									//clearea la interrupcion activando flag T0IR
	VICVectAddr = 0;							// Reconoce interrupcion escribiendo ahi un 0
}

void timer1_ISR (void) __irq{    // Generate Interrupt 
	if (timer1_routine != NULL)
		timer1_routine();
	
	T1IR = 1;									//clearea la interrupcion activando flag T0IR
	VICVectAddr = 0;							// Reconoce interrupcion escribiendo ahi un 0
}



// Test que comprueba el correcto funcionamiento de los test
int temporizador_hal_test(void) {
	unsigned i;
	temporizador_hal_iniciar();
	temporizador_hal_empezar();
	
	i = temporizador_hal_leer();
	while ((i + 50000) > temporizador_hal_leer()); 

	i = temporizador_hal_parar();
	if (i != temporizador_hal_leer()) return 1; //comprobamos que pare realmente
	
	return 0;
}


void temporizador_hal_iniciar(void) {
	timer0_count = 0;
	//T0PR (Timer 0 Prescale Register):  cu�ntos ciclos de reloj deben ocurrir antes de que el temporizador cuente 1 en su contador principal
	// valor mayor en TOPR har� que el temporizador cuente m�s lentamente, y un valor menor har� que cuente m�s r�pido.
	T0PR = 0;		//Cada ciclo va a contar un tick si lo poingo a 0
	
	//T0MR0 (Timer 0 Match Register 0): Cuando el T0TC alcanza el valor en T0MR0, se genera una interrupci�n (si est� habilitada
	T0MR0 = 0xFFFFFFFE;
	
	//T0MCR (Timer 0 Match Control Register): se utiliza para configurar el comportamiento del temporizador cuando se produce una coincidencia con el valor en T0MR0. 3: Genera una interrupción en la coincidencia.
	T0MCR = 3;                  // Timer0 Enable
	// 0x20 bit 5 enables vectored IRQs.
	// 4 is the number of the interrupt assigned. Number 4 is the Timer 0 (see table 40 of the LPC2105 user manual  
	VICVectCntl0 = 0x20 | 4;
	
	// configuration of the IRQ slot number 0 of the VIC for Timer 0 Interrupt
	VICVectAddr0 = (unsigned long)timer0_ISR;          // set interrupt vector in 0
}

// funci�n que inicia la cuenta de un contador de forma indefinida. 
void temporizador_hal_empezar(void){
	T0TCR = 1;		// Habilita el temporizador
	timer0_count = 0;
	// VICIntEnable = VICIntEnable | (1 << 4);
	VICIntEnable = VICIntEnable | 0x10;
}

// funci�n que lee el tiempo que  lleva contando el contador desde la �ltima
// vez que se ejecut� temporizador_hal_empezar  y lo devuelve en ticks. 
uint64_t temporizador_hal_leer(void){
	return timer0_count * (T0MR0 + 1) + T0TC; //T0TC suma los restantes
}

// detiene el contador y devuelve el  tiempo en ticks transcurrido desde el 
// �ltimo temporizador_hal_empezar. 
uint64_t temporizador_hal_parar(void){
	T0TCR = 0;
	return temporizador_hal_leer();
}

// función dependiente del hardware (timer1) que programa el reloj para que llame 
// a la función de callback cada periodo. El periodo se indica en ms. Si el periodo 
// es cero se para el temporizador.
void temporizador_hal_reloj (uint32_t periodo, void (*funcion_callback)()) {
  if (periodo == 0) {
		T1TCR = 0;
	}
	else {
			// Configurar el Timer 1 para generar interrupciones periódicas
		 T1MR0 = (periodo/temporizador_hal_tick2us) * 1000 - 1;  // Convertir el período a ticks y restar 1
		 T1MCR = 3;  // Generar interrupción y reiniciar el contador en la coincidencia
		 T1TCR = 1;  // Habilitar el temporizador
		 
		 // Configurar la rutina de interrupción para Timer 1
		 timer1_routine = funcion_callback;  // Asignar la dirección de la subrutina al vector de interrupción 5
		VICVectCntl5 = 0x20 | 5;  // Habilitar interrupción para Timer 1 en el vector 5
		VICVectAddr5 = (unsigned long) timer1_ISR;
		VICIntEnable = VICIntEnable | (1 << 5);  // Activar la interrupción del Timer 1
	}
}
