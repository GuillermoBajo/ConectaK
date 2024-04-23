#include <inttypes.h>

#include "evento.h"
#include "int_externas_hal.h"

#ifndef BOTONES_H
#define BOTONES_H

#define NUM_BOTONES 2
#define TIEMPO_USR_AUSENTE 12000


enum ESTADO {
    PULSADO,
    NO_PULSADO
};

/**
 * @brief Inicializa el módulo de botones.
 *
 * Esta función se encarga de inicializar el módulo de botones, estableciendo el estado inicial de los botones
 * y configurando las rutinas de interrupción correspondientes.
 *
 * @param callback_cola Puntero a la función que se ejecutará al encolar un evento.
 * @param ev_pulsacion Evento que se encolará al detectar una pulsación de botón.
 * @param callback_temporizador Puntero a la función que se ejecutará al activar un temporizador.
 * @param ev_temporizador Evento que se encolará al activar un temporizador.
 */
void botones_iniciar(void (*callback_cola)(), EVENTO_T ev_pulsacion,
                void (*callback_temporizador)(), EVENTO_T ev_temporizador);

/**
 * @brief Maneja la interrupción generada por los botones.
 *
 * Esta función se ejecuta cuando se produce una interrupción por pulsación de un botón.
 * Si el botón no estaba previamente pulsado, se encola un evento de pulsación y se actualiza el estado del botón.
 *
 * @param num_boton Número del botón que generó la interrupción.
 */
void interrupcion_eint(uint8_t num_boton);

/**
 * @brief Comprueba si algún botón está pulsado.
 *
 * Esta función comprueba si alguno de los botones está pulsado.
 * Si ninguno de los botones está pulsado, desactiva la comprobación y encola un evento de finalización.
 *
 * @return 1 si algún botón está pulsado, 0 en caso contrario.
 */
int boton_esta_pulsado(void);

/**
 * @brief Maneja la pulsación de un botón.
 *
 * Esta función debe ejecutar cuando se detecta una pulsación de botón.
 * Realiza las acciones correspondientes a la pulsación, como encolar un evento y actualizar el estado del botón.
 *
 * @param num_boton Número del botón que fue pulsado.
 */
void handle_pulsacion_boton(uint8_t num_boton);

/**
 * @brief Prueba el módulo de botones.
 *
 * Esta función realiza una prueba del módulo de botones, ejecutando una serie de pruebas internas.
 *
 * @return 0 si todas las pruebas pasan correctamente, un valor distinto de 0 en caso contrario.
 */
int botones_test(void);

#endif
