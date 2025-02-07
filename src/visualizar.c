 #include "visualizar.h"


void visualizar_iniciar(void) {
	// Configurar los pines del 23 al 16 como salidas
	 gpio_hal_sentido(GPIO_LATIDO, GPIO_LATIDO_BITS, GPIO_HAL_PIN_DIR_OUTPUT);
}

void visualizar_cuenta(int cuenta) {
	gpio_hal_escribir(GPIO_LATIDO, GPIO_LATIDO_BITS, cuenta);
}


void visualizar_hello_world(int temp) {
	gpio_hal_escribir(GPIO_HELLO_WORLD, GPIO_HELLO_WORLD_BITS, temp%(1 << 8));
}
