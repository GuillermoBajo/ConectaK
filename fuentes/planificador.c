#include "planificador.h"

static uint8_t powerDown;

void planificador() {
	
	temporizador_drv_iniciar();	// inicializa para contar tiempo de las alarmas
	temporizador_drv_empezar();
	
	FIFO_iniciar(GPIO_OVERFLOW);	// cola de eventos
	alarma_iniciar(FIFO_encolar, ALARMA_TRATAR);
	
	botones_iniciar(FIFO_encolar, PULSACION_BOTON, alarma_activar, COMPROBAR_BOTON);	// alarma cuando pulsa boton, 
	linea_serie_drv_iniciar(FIFO_encolar, ev_RX_SERIE, ev_TX_SERIE, CONTINUA_ENVIO);	// todo: añadir eventos

	hello_world_iniciar(FIFO_encolar, ev_LATIDO, ev_VISUALIZAR_HELLO);	// alarma periodica
	visualizar_iniciar();
	
	WD_hal_iniciar(WATCHDOG_TIMEOUT);
	powerDown = 0;
	alarma_activar(USUARIO_AUSENTE, TIEMPO_USR_AUSENTE, 0);

	juego_iniciar();	// alarma confirmacion jugada
	
	while (1) {
		EventoCola EC;
		uint32_t hayDato;
		hayDato = FIFO_extraer(&EC.ID_evento, &EC.auxData);
		if (hayDato) {
			WD_hal_feed();
			switch (EC.ID_evento) {										
				case ALARMA_TRATAR:
					alarma_tratar_evento();
					break;
				
				case ALARMA_OVERFLOW:
					FIFO_overflow();
					break;
				
				case PULSACION_BOTON:
					alarma_activar(USUARIO_AUSENTE, TIEMPO_USR_AUSENTE, 0);
					handle_pulsacion_boton(EC.auxData);
					if (powerDown) {
						powerDown = 0;
					
					} else {
						juego_tratar_evento(EC.ID_evento, EC.auxData);
					}
					break;
				
				// alarma
				case COMPROBAR_BOTON:
					boton_esta_pulsado();
					break;
									
				case ev_VISUALIZAR_CUENTA:
				 	visualizar_cuenta(EC.auxData);
				 	break;
				
				// alarma
				case ev_LATIDO:
				 	hello_world_tratar_evento();
				 	break;
				
				case ev_VISUALIZAR_HELLO:
					visualizar_hello_world(EC.auxData);
					break;
				
				// recibo de pantalla
				case ev_RX_SERIE:
					alarma_activar(USUARIO_AUSENTE, TIEMPO_USR_AUSENTE, 0);
					juego_tratar_evento(EC.ID_evento, EC.auxData);
					break;
				
				// acabo de escribir
				case ev_TX_SERIE:
					juego_tratar_evento(EC.ID_evento, EC.auxData);
					break;
				
				case CONTINUA_ENVIO:
					linea_serie_drv_continuar_envio();
					break;
				
				// alarma
				case JUGADA_CONFIRM_TEMP:
					juego_tratar_evento(EC.ID_evento, EC.auxData);
					break;
				
				// alarma
				case USUARIO_AUSENTE:
					powerDown = 1;
					power_hal_deep_sleep();
					break;
				
				default:
					break;
			}
		} else {
			power_hal_down();
		}
	}
}

