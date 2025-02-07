#include <inttypes.h>
#include <LPC210X.H>    // LPC21XX Peripheral Registers

#ifndef POWER_H
#define POWER_H

/**
 * @brief Cambia la frecuencia del procesador a 60 MHz.
 *
 * Función que configura el PLL para que la frecuencia del procesador sea de 60 MHz.
 */
extern void Switch_to_PLL(void);

/**
 * @brief Pone el sistema en modo de bajo consumo.
 *
 * Esta función configura los pines de interrupción externa (EXTINT1 y EXTINT2) para despertar el procesador
 * del modo de bajo consumo. Luego, activa el modo de bajo consumo en el registro PCON.
 */
void power_hal_down(void);

/**
 * @brief Pone el sistema en modo de sueño profundo.
 *
 * Esta función configura los pines de interrupción externa (EXTINT1 y EXTINT2) para despertar el procesador
 * del modo de sueño profundo. Luego, activa el modo de sueño profundo en el registro PCON y cambia a la frecuencia
 * del PLL para un rápido restablecimiento del sistema al despertar.
 */
void power_hal_deep_sleep(void);

#endif
