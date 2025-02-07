#include "power.h"

void power_hal_down(void) {
	EXTWAKE = 6; // EXTINT1 y 2 (correspondientes a EINT1 y EINT2) will awake the processor		
	PCON = (PCON | 0x1);
}

void power_hal_deep_sleep(void) {
	EXTWAKE = 6; // EXTINT1 y 2 (correspondientes a EINT1 y EINT2) will awake the processor	
 	PCON = (PCON | 0x2);
	Switch_to_PLL();
}
