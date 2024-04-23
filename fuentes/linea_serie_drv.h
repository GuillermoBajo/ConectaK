#include "system_calls.h"
#include "linea_serie_hal.h"
#include "gpio.h"
#include "evento.h"
#include "utils.h"

#ifndef LINEA_SERIE_DRV_H
#define LINEA_SERIE_DRV_H

static void (*callback_encolar)(EVENTO_T ID_evento, uint32_t auxData);

enum ESTADO_RECIBIR {
	ALM,
	WAIT,
	LEN_ERROR,
};

enum ESTADO_ENVIAR {
	FIN,
	ENVIAR,
};

/**
 * @brief Inicia el controlador de la línea serie.
 *
 * Esta función inicializa el controlador de la línea serie.
 * Configura los eventos de finalización de recepción, finalización de mostrar y continuación de mostrar.
 * También configura la función de callback para encolar eventos.
 *
 * @param funcion_callback La función de callback para encolar eventos.
 * @param finRecibir El evento de finalización de recepción.
 * @param finMostrar El evento de finalización de mostrar.
 * @param continuaMostrar El evento de continuación de mostrar.
 */
void linea_serie_drv_iniciar(void (*funcion_callback)(), EVENTO_T finRecibir, 
											EVENTO_T finMostrar, EVENTO_T continuaMostrar);

/**
 * @brief Función de interrupción de la línea serie.
 *
 * Esta función se llama cuando se produce una interrupción en la línea serie.
 * Analiza el estado actual de recepción y realiza las acciones correspondientes.
 *
 * @param caracter El caracter recibido.
 */
void interrupcion_serie(uint8_t caracter);

/**
 * @brief Envía un array a través de la línea serie.
 *
 * Esta función envía un array de caracteres a través de la línea serie.
 * El envío se realiza de forma asíncrona, es decir, la función no bloquea.
 *
 * @param salida El array de caracteres a enviar.
 */
void linea_serie_drv_enviar_array(char salida[]);
/**
 * @brief Continúa el envío de datos a través de la línea serie.
 *
 * Esta función continúa el envío de datos a través de la línea serie.
 * Después de enviar todos los datos, se envía un carácter de nueva línea y se llama al evento de finalización de mostrar.
 */
void linea_serie_drv_continuar_envio(void);

#endif
