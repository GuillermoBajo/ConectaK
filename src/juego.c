#include "juego.h"

static uint8_t primera_partida = 1;

static TABLERO cuadricula;
static uint8_t turno, ganador, num_jugadas, posicion_ocupada;

//Variables automata
char info_juego[] = "Conecta K:\nGana el primero que consiga K fichas en linea\n--------------------------------------------\nComandos:\n $NEW!: Iniciar un nuevo juego\n $END!: Terminar el juego\n $#-#!: Fila-Columna en la que queremos poner la ficha\n";
static enum ESTADO_CONECTAK estado;
static uint32_t estadisticas_fifo[NUM_TIPO_EVENTOS];

//Input
static uint32_t boton_data;
static uint8_t boton_pulsado;
static enum COMANDO comando_data;
static uint8_t comando_recibido;
static uint8_t hay_tx;

//Control automata
static uint8_t columna_jugada = 0;
static uint8_t fila_jugada = 0;
static uint8_t jugada_confirmada = 0;

//Variables para las estadisticas
static uint64_t tiempo_total_procesador, tiempo_parc_procesador;

static uint64_t tiempo_total_hay_linea, tiempo_parc_hay_linea;
static uint32_t llamadas_hay_linea;

static uint64_t tiempo_total_humano, tiempo_parc_humano;

char buffer[1200];
char buff_itoa[6];
char tablero[200];

void juego_iniciar(void) {
	int i;
	estado = INICIO;
	conecta_K_automata();
	for (i=0; i<NUM_TIPO_EVENTOS; i++) {
		estadisticas_fifo[i] = 0;
	}
}


void juego_reiniciar_partida(void){
	turno = 2;
	ganador = 0;
	num_jugadas = 0;
	posicion_ocupada = 0;
	
	boton_data = 0;
	boton_pulsado = 0;
	comando_data = NOC;
	comando_recibido = 0;
	hay_tx = 0;	
	columna_jugada = 0;
	fila_jugada = 0;
	jugada_confirmada = 0;
	
	tablero_inicializar(&cuadricula);
	if (primera_partida == 1) {
		conecta_K_test_cargar_tablero(&cuadricula);
		primera_partida = 0;
	}
	tiempo_parc_procesador = temporizador_drv_leer();
	tiempo_total_hay_linea = 0;
	tiempo_total_humano = 0;
}

 void juego_tratar_evento(EVENTO_T ID_evento, uint32_t auxData){
	if (ID_evento == PULSACION_BOTON) {
		boton_data = auxData;
		boton_pulsado = 1;
	}
	else if (ID_evento == ev_RX_SERIE) {
		sel_comando(auxData);
		comando_recibido = 1;
	}
	else if (ID_evento == ev_TX_SERIE){
		hay_tx = 1;
	}
	else if (ID_evento == JUGADA_CONFIRM_TEMP){
		jugada_confirmada = 1;
	}
	conecta_K_automata();
}


void conecta_K_automata(void){
	ms(buffer, 1200, '\0');
	
	if(estado == INICIO){
		juego_reiniciar_partida();
		linea_serie_drv_enviar_array(info_juego);
		estado = INTERM_INI;
	}
	
	else if(estado == INTERM_INI && hay_tx){
		hay_tx = 0;
		estado = ESPERA_PARTIDA;
	}

	else if(estado == ESPERA_PARTIDA && (comando_recibido && comando_data == NEW)){
		strcpy(buffer, conecta_K_visualizar_tablero());
		itoa(turno, buff_itoa);
		strcat(buffer, "Turno de jugador ");
		strcat(buffer, buff_itoa);
		strcat(buffer, "...\n");
		
		linea_serie_drv_enviar_array(buffer);
		
		comando_recibido = 0;
		comando_data = NOC;	
		estado = INTERM_NEW;
	}

	else if(estado == INTERM_NEW && hay_tx){
		hay_tx = 0;
		estado = ESPERA_COMANDO;
		tiempo_parc_humano = temporizador_drv_leer();
	}

	else if(estado == ESPERA_COMANDO){
		// CASO PULSACION BOTON 2 PARA TERMINAR PARTIDA
		if (boton_pulsado && boton_data == 2) {
			crear_msjFin(buffer, "boton2");
			linea_serie_drv_enviar_array(buffer);
			
			boton_data = 0;
			boton_pulsado = 0;
			estado = INTERM_END_COM;
			turno = (turno == 1) ? 2 : 1;
		}
		// CASO COMANDO RECIBIDO
		else if(comando_recibido){
			if(comando_data == ERR){
				if(columna_jugada == 0){
					strcpy(buffer, "Error: Columna invalida\n");
				}
				else if(fila_jugada == 0){
					strcpy(buffer, "Error: Fila invalida\n");
				}
				else {
					strcpy(buffer, "Error. Comando desconocido: ");
					strcat(buffer, comandoToString(comando_data));
					strcat(buffer, "\n");
				}
				linea_serie_drv_enviar_array(buffer);
				gpio_hal_escribir(GPIO_COMANDO_ERRONEO, GPIO_COMANDO_ERRONEO_BITS, 1);
				estado = INTERM_ERR;
			}

			else if(comando_data == END){
				gpio_hal_escribir(GPIO_COMANDO_ERRONEO, GPIO_COMANDO_ERRONEO_BITS, 0);
				
				crear_msjFin(buffer, "end");
				linea_serie_drv_enviar_array(buffer);

				estado = INTERM_END_CONF;
			}

			else if(comando_data == JUG){
				tiempo_total_humano += temporizador_drv_leer() - tiempo_parc_humano;
				gpio_hal_escribir(GPIO_COMANDO_ERRONEO, GPIO_COMANDO_ERRONEO_BITS, 0);
				
				conecta_K_jugada(fila_jugada-1, columna_jugada-1, 3); // mostar tablero provisional
				if (posicion_ocupada) {
					strcpy(buffer, "Error: Posicion ocupada. Pon la ficha en una casilla libre\n");
					estado = INTERM_ERR;

				} else {
					strcpy(buffer, conecta_K_visualizar_tablero());
					strcat(buffer, "Pulsa boton 1 para cancelar...\n");
					
					tablero_borrar_celda(&cuadricula, fila_jugada-1, columna_jugada-1);
					alarma_activar(JUGADA_CONFIRM_TEMP, 3000, 0);
					estado = INTERM_JUG;
				}
				linea_serie_drv_enviar_array(buffer);
			}

			// Se resetean estas variables se haya recibido el comando que sea: 
			comando_recibido = 0;
			comando_data = NOC;
		}
	}

	else if(estado == ESPERA_CONFIRMACION){
		if((boton_pulsado && boton_data == 2) || (comando_recibido && comando_data == END)){
			estado = INTERM_END_CONF;
			//Reseteo variables	
			if(boton_pulsado){
				crear_msjFin(buffer, "boton2");
				
				boton_pulsado = 0;
				boton_data = 0;
			}
			if(comando_recibido){
				crear_msjFin(buffer, "end");
				
				comando_recibido = 0;
				comando_data =NOC;
			}
			linea_serie_drv_enviar_array(buffer);
		}
		
		else if(boton_pulsado && boton_data == 1){
			strcpy(buffer, "Jugada cancelada. Inserte una nueva jugada...\n");
			linea_serie_drv_enviar_array(buffer);
			estado = INTERM_CANC_JUG;
			boton_pulsado = 0;
			boton_data = 0;
		}
		
		
		else if (jugada_confirmada){
			// borrar celda
			conecta_K_jugada(fila_jugada-1 , columna_jugada-1, turno);
			strcpy(buffer, conecta_K_visualizar_tablero());
			
			fila_jugada = 0;
			columna_jugada = 0;
			num_jugadas++;

			if (ganador){
				crear_msjFin(buffer, "ganador");
				estado = INTERM_END_CONF;
			
			} else {
				turno = (turno == 1) ? 2 : 1;
				itoa(turno, buff_itoa);
				strcat(buffer, "Turno de jugador ");
				strcat(buffer, buff_itoa);
				strcat(buffer, "...\n");

				estado = INTERM_CONFIRM;
			}
			linea_serie_drv_enviar_array(buffer);
			jugada_confirmada = 0;		
		}
	} 
	
	else if(estado == INTERM_ERR && hay_tx){
		hay_tx = 0;
		estado = ESPERA_COMANDO;
	}
	
	else if(estado == INTERM_END_COM && hay_tx){
		hay_tx = 0;
		estado = INICIO;
		conecta_K_automata();
	}

	else if(estado == INTERM_JUG && hay_tx){
		hay_tx = 0;
		estado = ESPERA_CONFIRMACION;
	}

	else if(estado == INTERM_CONFIRM && hay_tx){
		hay_tx = 0;
		estado = ESPERA_COMANDO;
	}

	else if(estado == INTERM_CANC_JUG && hay_tx){
		hay_tx = 0;
		estado = ESPERA_COMANDO;
	}


	else if(estado == INTERM_END_CONF && hay_tx){
		hay_tx = 0;
		estado = INICIO;
		conecta_K_automata();
	}
}

char* conecta_K_visualizar_tablero(void) {
	uint8_t f, c;
	char caracter;
	int fila_ancho = (NUM_COLUMNAS+1)*2+1;
	
	for (f = 0; f < NUM_FILAS + 1; ++f) {
		for (c = 0; c < NUM_COLUMNAS + 1; ++c) {
			if (f == 0) {
				if (c == 0) {
					caracter = '-';
				
				} else {
					caracter = c + 48;
				}
			} else if (c == 0) {
				caracter = f  + 48;
			
			}	else {
				uint8_t contenido_celda = tablero_leer_celda(&cuadricula, f - 1, c - 1);
				if (contenido_celda == 0x04) {
					caracter = '#';
					
				} else if (contenido_celda == 0x05) {
					caracter = 'B';
					
				} else if (contenido_celda == 0x06) {
					caracter = 'N'; // 4e
					
				} else {
					caracter = ' '; // 20
				}
			}
			tablero[f*fila_ancho + c*2] = caracter;
			tablero[f*fila_ancho + c*2 +1] = '|';
		}
		tablero[f*fila_ancho + c*2] = '\n';
	}
	return tablero;
}


void conecta_K_jugada(uint8_t row, uint8_t column, uint8_t colour){

	if(tablero_fila_valida(row) && tablero_columna_valida(column) && tablero_color_valido(colour)){	
		//podriamos no validarla ya que tablero_insertar_valor vuelve a validar
		if (celda_vacia(tablero_leer_celda(&cuadricula, row, column))){
			//tablero_insertar tambien chequea si esta libre esa celda o no...
			posicion_ocupada = 0;
			if(tablero_insertar_color(&cuadricula, row, column, colour) == EXITO) {				
				tiempo_parc_hay_linea = temporizador_drv_leer();
				llamadas_hay_linea++;
				ganador = conecta_K_hay_linea_arm_arm(&cuadricula, row, column, colour);
				tiempo_total_hay_linea += temporizador_drv_leer() - tiempo_parc_hay_linea;				
			
			} else {
				// no ha insertado bien
				while(1); //no cabe en la matriz dispersa, hemos dimensionado mal, error de dise�o
			}
		
		} else {
			//celda no vacia
			posicion_ocupada = 1;
		}
		
	} else {
		// fuera de rango fila, columna o color
		while(1);
	}
}

void crear_msjFin(char* dest, const char* parametro) {
	int i, numEventos;
	tiempo_total_procesador = temporizador_drv_leer() - tiempo_parc_procesador;
	
	strcat(dest, "FIN DE LA PARTIDA: El jugador ");
	itoa(turno, buff_itoa);
	strcat(dest, buff_itoa);
	
	if (strcmp(parametro, "boton2") == 0) {
		strcat(dest, " ha pulsado el boton 2.\n");
	} else if (strcmp(parametro, "end") == 0) {
		strcat(dest, " ha introducido el comando END.\n");
	} else if (strcmp(parametro, "ganador") == 0){
		strcat(dest, " ha ganado.\n");
	} else {
		strcat(dest, "-------ERROR-------");
	}

	strcat(dest, "Tiempo de uso del procesador:\nTotal: ");
	itoa(tiempo_total_procesador/1000000, buff_itoa);	// pasamos a segundos
	strcat(dest, buff_itoa);
	
	strcat(dest, " s\n\nTiempo de computo de conecta_K_hay_linea:\nTotal: ");
	itoa(tiempo_total_hay_linea, buff_itoa);
	strcat(dest, buff_itoa);
	strcat(dest, " us\nMedia: ");
	itoa(tiempo_total_hay_linea/llamadas_hay_linea, buff_itoa);
	strcat(dest, buff_itoa);
	
	strcat(dest, " us\n\nTiempo que al humano le cuesta pensar la jugada:\nTotal: ");
	itoa(tiempo_total_humano/1000, buff_itoa);
	strcat(dest, buff_itoa);
	strcat(dest, " ms\nMedia: ");
	itoa((tiempo_total_humano/num_jugadas)/1000, buff_itoa);
	strcat(dest, buff_itoa);
	strcat(dest, " ms\n\nEventos esta partida: \n");
	
	for (i = 0; i<NUM_TIPO_EVENTOS; i++) {
		strcat(dest, eventoToString((EVENTO_T)i));
		strcat(dest, ": ");
		
		numEventos = FIFO_estadisticas((EVENTO_T)i);	
		itoa(numEventos - estadisticas_fifo[i], buff_itoa);
		estadisticas_fifo[i] = numEventos;
		
		strcat(dest, buff_itoa);
		strcat(dest, "\n");
	}
}

void conecta_K_test_cargar_tablero(TABLERO *t) {
	size_t f, c;
	#include "tablero_test.h"	 

	for(f = 0; f < NUM_FILAS; ++f){
		for(c = 0; c < NUM_COLUMNAS; ++c) {
			tablero_insertar_color (t, f, c, tablero_test[f][c]);
		}
	}
}


void sel_comando(uint32_t comando) {
	fila_jugada = 0;
	columna_jugada = 0;
	
	if (atoi("NEW") == comando)
		comando_data = NEW;
	else if (atoi("END") == comando)
		comando_data = END;		
	else {
		itoa(comando, buff_itoa);
		buff_itoa[1] = 0;	// torpeza. cambiar?
		fila_jugada = atoi(&buff_itoa[0]);
		columna_jugada = atoi(&buff_itoa[2]);
		
		if (fila_jugada > 0 && fila_jugada <= 7 && 
			columna_jugada > 0 && columna_jugada <= 7) {
			comando_data = JUG;
		
		} else {
			comando_data = ERR;
		}		
	}
}

char* eventoToString(EVENTO_T evento) {
	switch (evento) {
		case VOID: return "TOTAL EVENTOS";
		case TIMER0: return "TIMER0";
		case TIMER1: return "TIMER1";  
		case ALARMA_OVERFLOW: return "ALARMA_OVERFLOW";
		case ALARMA_TRATAR: return "ALARMA_TRATAR";
		case PULSACION_BOTON: return "PULSACION_BOTON";
		case COMPROBAR_BOTON: return "COMPROBAR_BOTON";
		case USUARIO_AUSENTE: return "USUARIO_AUSENTE";
		case ev_VISUALIZAR_CUENTA: return "ev_VISUALIZAR_CUENTA";
		case ev_LATIDO: return "ev_LATIDO";
		case ev_VISUALIZAR_HELLO: return "ev_VISUALIZAR_HELLO";
		case ev_TX_SERIE: return "ev_TX_SERIE";
		case ev_RX_SERIE: return "ev_RX_SERIE";
		case CONTINUA_ENVIO: return "CONTINUA_ENVIO";
		default: return "ERROR";
	}
}

char* comandoToString(enum COMANDO com) {
	switch (com) {
		case NOC: return "NOC";
		case END: return "END";
		case NEW: return "NEW";  
		case ERR: return "ERR";
		case JUG: return "JUG";
		default: return "ERR";
	}
}


//void juego_visualizar_tiempo(void) {
//	char salida[10];

//	if (visualizar_end == visualizar_init) while(1);

//	visualizar_end = clock_getus();
//	intToCharArray(visualizar_end-visualizar_init, salida);
//	
//	linea_serie_drv_enviar_array(salida);
//	visualizar_end = 0;
//	visualizar_init = 0;
//}
