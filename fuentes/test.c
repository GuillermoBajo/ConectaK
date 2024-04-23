#include "test.h"

int test_all(void) {
	uint8_t test;
	
	test = botones_test();
	if (test)	return 1;

	test = timer0_driver_test();
	if (test)	return 1; 

	test = FIFO_test(0);
	if (test)	return 1;

	test = gpio_hal_test();
	if (test)	return 1;

	test = alarmas_test(FIFO_encolar, FIFO_iniciar);
	if (test)	return 1;
		
	return 0;	
}

void test_watchdog(void) {
	WD_hal_test();
}
