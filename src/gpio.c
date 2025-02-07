#include "gpio.h"


// Funcion que prueba el distinto funcionamiento de las funciones del modulo GPIO
int gpio_hal_test(void){
	int test;
	
	// Inicializar el sistema
	gpio_hal_iniciar();

	gpio_hal_sentido(0, 4, GPIO_HAL_PIN_DIR_OUTPUT);

	test = gpio_hal_leer(0, 4);     // deberia leer 0 porque aun no se ha escrito
	if (test != 0) return 1;

  gpio_hal_escribir(0, 2, 7);  // escribe todos los pines del 0 al 2. no se deberia exceder
	test = gpio_hal_leer(0, 4);
  if (test != 3) return 1;

  gpio_hal_escribir(0, 4, 0);  // borra todos los valores
  test = gpio_hal_leer(0, 4);  
	if (test != 0) return 1;

  gpio_hal_escribir(0, 4, 4);  // escribe un 4
  test = gpio_hal_leer(0, 4);  
	if (test != 4) return 1;

    return 0;
}	
