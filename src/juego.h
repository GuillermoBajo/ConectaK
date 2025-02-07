#include <inttypes.h>

#include "linea_serie_drv.h"
#include "timer0_driver.h"
#include "tablero.h"
#include "celda.h"
#include "config_conecta_K.h"
#include "alarmas.h"
#include "utils.h"

#ifndef JUEGO_H
#define JUEGO_H

enum COMANDO {
	NOC,
	END,
	NEW,
	TAB,
	JUG,
	ERR,
};

enum ESTADO_CONECTAK {
	INICIO = 0,
	INTERM_INI = 1,
	ESPERA_PARTIDA = 2,
	INTERM_NEW = 3,
	ESPERA_COMANDO = 4,
	INTERM_ERR = 5,
	INTERM_JUG = 6,
	ESPERA_CONFIRMACION = 7,
	INTERM_CONFIRM = 8,
	INTERM_CANC_JUG = 9,
	INTERM_END_COM = 10,
	INTERM_END_CONF = 11,
};

/**
 * @brief Inicializa el juego.
 * 
 * Esta función se encarga de inicializar todas las variables y estructuras necesarias para comenzar el juego
 * por primera vez.
 */
void juego_iniciar(void);

/**
 * @brief Trata un evento del juego.
 * 
 * Esta función se encarga de tratar un evento del juego, como una pulsación de botón o un comando recibido.
 * 
 * @param ID_evento El identificador del evento.
 * @param auxData Datos adicionales asociados al evento.
 */
void juego_tratar_evento(EVENTO_T ID_evento, uint32_t auxData);

/**
 * @brief Ejecuta el autómata del juego.
 * 
 * Esta función ejecuta el autómata del juego, controlando el flujo del juego y realizando las acciones correspondientes a cada estado.
 */
void conecta_K_automata(void);

/**
 * @brief Visualiza el tablero de juego.
 * 
 * Esta función genera una representación visual del tablero de juego y la devuelve como una cadena de caracteres.
 * 
 * @return La representación visual del tablero de juego.
 */
char* conecta_K_visualizar_tablero(void);

/**
 * @brief Realiza una jugada en el juego.
 * 
 * Esta función realiza una jugada en el juego, insertando una ficha en la posición especificada.
 * Si la jugada es válida, se inserta la ficha y se comprueba si hay una línea de cuatro fichas del mismo color.
 * Si hay ganador, "ganador"=1. Si no hay ganador, "ganador"=0.
 * 
 * @param row La fila en la que se desea realizar la jugada.
 * @param column La columna en la que se desea realizar la jugada.
 * @param colour El color de la ficha a insertar.
 */
void conecta_K_jugada(uint8_t row, uint8_t column, uint8_t colour);

/**
 * @brief Comprueba si hay una línea de cuatro fichas del mismo color.
 * 
 * Esta función comprueba si hay una línea de cuatro fichas del mismo color. Corresponde a la cabezera
 * de la función implementada en ensamblador.
 * 
 * @param t El tablero de juego.
 * @param row La fila en la que se ha realizado la jugada.
 * @param column La columna en la que se ha realizado la jugada.
 * @param colour El color de la ficha que se ha insertado.
 */
uint8_t conecta_K_hay_linea_arm_arm(TABLERO *t, uint8_t fila, uint8_t columna, uint8_t color);


void juego_visualizar_tiempo(void);
void conecta_K_test_cargar_tablero(TABLERO *t);
void crear_msjFin(char* dest, const char* parametro);
void sel_comando(uint32_t comando);
char* eventoToString(EVENTO_T evento);
char* comandoToString(enum COMANDO com);

#endif
