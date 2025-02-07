#include <inttypes.h>
#include <LPC210X.H>    // LPC21XX Peripheral Registers

#include "gpio_reserva.h"

#ifndef GPIO_H
#define GPIO_H

// Permite emplear el GPIO y debe ser invocada
// antes de poder llamar al resto de funciones de la biblioteca.
__inline static void
gpio_hal_iniciar() {
    PINSEL0 = 0; // se están configurando todos los pines en ese registro como pines GPIO, es decir, deshabilita cualquier otra función especial que puedan tener
    PINSEL1 = 0;
		//gpio_hal_sentido(0, 32, GPIO_HAL_PIN_DIR_OUTPUT);
		//gpio_hal_escribir(0, 32, 0);
}

// los bits indicados se utilizarán como 
// entrada o salida según la dirección.
__inline static void
gpio_hal_sentido(GPIO_HAL_PIN_T gpio_inicial, uint8_t num_bits, gpio_hal_pin_dir_t direccion) {
    // 1 es output, 0 es input
    // igual que leer, si 
    uint32_t mask = (1u << num_bits) - 1;
    if (direccion == GPIO_HAL_PIN_DIR_INPUT) IODIR &= ~(mask << gpio_inicial);
    else IODIR |= mask << gpio_inicial;
}

// gpio_inicial indica el primer bit a leer, num_bits indica 
// cuántos bits queremos leer. La función devuelve un entero con el valor 
// de los bits indicados. Ejemplo:
//      o valor de los pines: 0x0F0FAFF0
//      o bit_inicial: 12 num_bits: 4
//      o valor que retorna la función: 10 (lee los 4 bits 12-15)
__inline static uint32_t
gpio_hal_leer(GPIO_HAL_PIN_T gpio_inicial, uint8_t num_bits) {
  uint32_t mask = (1u << num_bits) - 1;  
	return (IOPIN >> gpio_inicial) & mask;
}

// similar al anterior, pero en lugar de leer escribe en los 
// bits indicados el valor (si valor no puede representarse en los bits 
// indicados se escribirá los num_bits menos significativos a partir del inicial)
__inline static void
gpio_hal_escribir(GPIO_HAL_PIN_T bit_inicial, uint8_t num_bits, uint32_t valor) {
    uint32_t mask = (1u << num_bits) - 1;
    IOCLR = mask << bit_inicial;
    IOSET = (valor & mask) << bit_inicial; 
}


int gpio_hal_test(void);	


#endif
