#include <stdint.h>

#include "fifo.h"
#include "hello_world.h"
#include "power.h"
#include "alarmas.h"
#include "botones.h"
#include "juego.h"
#include "visualizar.h"
#include "linea_serie_drv.h"
#include "watchdog.h"
#include "evento.h"


#ifndef PLANIFICADOR_H
#define PLANIFICADOR_H

#define WATCHDOG_TIMEOUT 1
/**
 * @brief Gestiona los eventos del sistema.
 * 
 * Esta función se encarga de gestionar los eventos del sistema, ejecutando las funciones asociadas a cada evento.
 */
void planificador(void);

#endif
