#include <inttypes.h>

#include "io_reserva.h"

#ifndef GPIO_RES_H
#define GPIO_RES_H

// Definición del tipo de dirección del pin del GPIO
typedef enum
{
    // Fast GPIO Port Pin value register using FIOMASK. The current state of digital port pins can be read from this register,.
    GPIO_HAL_PIN_DIR_INPUT,
    // Fast GPIO Port Output Set register using FIOMASK. This register controls the state of output pins.Writing 1s produces HIGH 
    // at the corresponding port pins.Writing 0s has no effect.
    GPIO_HAL_PIN_DIR_OUTPUT
} gpio_hal_pin_dir_t;

// Definición del tipo de representación del pin del GPIO
typedef uint8_t GPIO_HAL_PIN_T;

#endif
