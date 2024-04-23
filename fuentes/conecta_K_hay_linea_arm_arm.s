			AREA datos, DATA, READWRITE
delF			DCB 	0x00, 0xFF, 0xFF, 0x01
delC			DCB 	0xFF, 0x00, 0xFF, 0xFF
K_SIZE			EQU 	4
	
NUM_FILAS		EQU		7
NUM_COLUMNAS	EQU		7
N_DELTAS		EQU		4
MAX_NO_CERO		EQU 	6
	
	
		AREA codigo, CODE, READONLY
		EXPORT conecta_K_hay_linea_arm_arm
		PRESERVE8

conecta_K_hay_linea_arm_arm
		mov 	IP, SP
		stmdb	SP!,  {r4-r10,FP,IP,LR,PC}
		sub 	FP, IP, #4		
	
		mov r9, #0				;linea = FALSE (mantiene seguimiento de linea mayor que K)
		mov r10, #0				;long_linea = 0 (long linea actual que se mira)
		

primera_iter_buc		
		ldr r4, =delF			
		ldr r5, =delC
		ldrsb r7, [r4, #0]		
		ldrsb r8, [r5, #0]
		stmdb SP!, {r9-r10}		;Apilo EN pila regs que se van a soberescribir y no puedo obtener de memoria (no como r4 y r5 que es un ldr y ya)
		mov	r9, r1
		mov r10, r2
		;---------------------------------------------------------------- COMIENZO BA1_1 -------------------------------------------------------------------
		; r0=t, r1=fila, r2=columna, r3=color, r5=MAX_NO_CERO, r6=longitud, r7=TEMP:delta_fil, r8=TEMP:delta_col, r9=TEMP:nueva_fila, r10=TEMP:nueva_columna
		mov r5, #MAX_NO_CERO	
		mov	r6, #0			; longitud = 0
		
while_1BA1_1
		cmp		r9, #NUM_FILAS
		bhs		endwhile_1BA1_1
		cmp 	r10, #NUM_COLUMNAS
		bhs		endwhile_1BA1_1
		; r4=TEMP:col, r11=TEMP:&tablero->columna, r12=TEMP:valor tabl->[fila][col]
		mov 	r4, #0
		mla		r11, r9, r5, r0	; addr del vector COLUMNA(t->columnas[nueva_fila])
		
while_2BA1_1	
		cmp 	r4, r5			; if(col == MAX_NO_CERO)
		bhs		endwhile_1BA1_1		; return longitud
		ldrb 	r12, [r11, r4]	; r4 = (t->columnas[fila]r11)[col(r4)] no hay desplazamiento (uint8->1B)
		cmp 	r12, r10			; si son iguales ya esta encontrado		
		addne 	r4, r4, #1		; col++ solo si son distintos
		bne		while_2BA1_1	; vuelve si son distintos
endwhile_2BA1_1
		; r11, r12 out

check_colorBA1_1		; r11:@t->no_cero ,r12=NUM_FILAS y color
		mov		r12, #NUM_FILAS
		; si hubiera registros de sobra, poner la inst de abajo al principio
		mla		r11, r12, r5, r0	; r9=(MAX_NO_CERO*NUM_FILAS)+t ME HE SALTADO COLUMNAS
		mla		r11, r9, r5, r11	; nos movemos a la fila de la matriz no_cero necesaria (tienen max_cero comp)
		ldrb 	r4, [r11, r4]		; (t->no_ceros[fila]r11)[col(r4)]
		and 	r12, r4, #0x03		; and celda & 0x03
		cmp		r12, r3				; r7 == r3
		bne 	endwhile_1BA1_1				; si es igual-> exito
		; mirar si se pueden cargar aqui los deltas poniendole flags a add y cargandolo si r6 es 0
		add 	r6, r6, #1		; ++longitud
		add 	r9, r9, r7		; nueva_fila += delta_fila
		add 	r10, r10, r8	; nueva_columna += delta_columna
		b 		while_1BA1_1
endwhile_1BA1_1
;---------------------------------------------------------------- FIN BA1_1 -------------------------------------------------------------------
		ldmia SP!, {r9-r10}		;Recuperamos valores de registros y los guardamos en r9 y r10
		
		mov r10, r6
		cmp r10, #K_SIZE			;Comprobamos el resultado obtenido en buscar alineamiento
		movge r9, #1				;linea = TRUE
		bge finHL
		
		rsb r8, r8, #0				;-deltaF[i]
		rsb r7, r7, #0				;-deltaC[i]	
		stmdb SP!, {r9-r10}
		
		add r9, r1, r7
		add r10, r2, r8
		
;---------------------------------------------------------------- COMIENZO BA1_2 --------------------------------------------------------------------
		; r0=t, r1=fila, r2=columna, r3=color, r5=MAX_NO_CERO, r6=longitud, r7=TEMP:delta_fil, r8=TEMP:delta_col, r9=TEMP:nueva_fila, r10=TEMP:nueva_columna
		mov r5, #MAX_NO_CERO	
		mov	r6, #0			; longitud = 0
		
while_1BA1_2
		cmp		r9, #NUM_FILAS
		bhs		endwhile_1BA1_2
		cmp 	r10, #NUM_COLUMNAS
		bhs		endwhile_1BA1_2
		; r4=TEMP:col, r11=TEMP:&tablero->columna, r12=TEMP:valor tabl->[fila][col]
		mov 	r4, #0
		mla		r11, r9, r5, r0	; addr del vector COLUMNA(t->columnas[nueva_fila])
		
while_2BA1_2	
		cmp 	r4, r5			; if(col == MAX_NO_CERO)
		bhs		endwhile_1BA1_2		; return longitud
		ldrb 	r12, [r11, r4]	; r4 = (t->columnas[fila]r11)[col(r4)] no hay desplazamiento (uint8->1B)
		cmp 	r12, r10			; si son iguales ya esta encontrado
		;beq 	endwhile_2			
		addne 	r4, r4, #1		; col++ solo si son distintos
		bne		while_2BA1_2	; vuelve si son distintos
endwhile_2BA1_2
		; r11, r12 out

check_colorBA1_2		; r11:@t->no_cero ,r12=NUM_FILAS y color
		mov		r12, #NUM_FILAS
		; si hubiera registros de sobra, poner la inst de abajo al principio
		mla		r11, r12, r5, r0	; r9=(MAX_NO_CERO*NUM_FILAS)+t ME HE SALTADO COLUMNAS
		mla		r11, r9, r5, r11	; nos movemos a la fila de la matriz no_cero necesaria (tienen max_cero comp)
		ldrb 	r4, [r11, r4]		; (t->no_ceros[fila]r11)[col(r4)]
		and 	r12, r4, #0x03		; and celda & 0x03
		cmp		r12, r3				; r7 == r3
		bne 	endwhile_1BA1_2				; si es igual-> exito
		; mirar si se pueden cargar aqui los deltas poniendole flags a add y cargandolo si r6 es 0
		add 	r6, r6, #1		; ++longitud
		add 	r9, r9, r7		; nueva_fila += delta_fila
		add 	r10, r10, r8	; nueva_columna += delta_columna
		b 		while_1BA1_2
endwhile_1BA1_2
;---------------------------------------------------------------- FIN BA1_2 --------------------------------------------------------------------

		ldmia SP!, {r9-r10}	;Recuperamos valores de registros y los guardamos en r9 y r10
		
		add r10, r10, r6
		cmp r10, #K_SIZE	;Comprobamos el resultado obtenido en buscar alineamiento
		movge r9, #1
		bge finHL			;Pq linea = TRUE


segunda_iter_buc		
		ldr r4, =delF			
		ldr r5, =delC
		ldrsb r7, [r4, #1]		
		ldrsb r8, [r5, #1]
		stmdb SP!, {r9-r10}		;Apilo EN pila regs que se van a soberescribir y no puedo obtener de memoria (no como r4 y r5 que es un ldr y ya)
		mov	r9, r1
		mov r10, r2

		;---------------------------------------------------------------- COMIENZO BA2_1 -------------------------------------------------------------------
		; r0=t, r1=fila, r2=columna, r3=color, r5=MAX_NO_CERO, r6=longitud, r7=TEMP:delta_fil, r8=TEMP:delta_col, r9=TEMP:nueva_fila, r10=TEMP:nueva_columna
		mov r5, #MAX_NO_CERO	
		mov	r6, #0			; longitud = 0
		
while_1BA2_1
		cmp		r9, #NUM_FILAS
		bhs		endwhile_1BA2_1
		cmp 	r10, #NUM_COLUMNAS
		bhs		endwhile_1BA2_1
		; r4=TEMP:col, r11=TEMP:&tablero->columna, r12=TEMP:valor tabl->[fila][col]
		mov 	r4, #0
		mla		r11, r9, r5, r0	; addr del vector COLUMNA(t->columnas[nueva_fila])
		
while_2BA2_1
		cmp 	r4, r5			; if(col == MAX_NO_CERO)
		bhs		endwhile_1BA2_1		; return longitud
		ldrb 	r12, [r11, r4]	; r4 = (t->columnas[fila]r11)[col(r4)] no hay desplazamiento (uint8->1B)
		cmp 	r12, r10			; si son iguales ya esta encontrado
		;beq 	endwhile_2			
		addne 	r4, r4, #1		; col++ solo si son distintos
		bne		while_2BA2_1	; vuelve si son distintos
endwhile_2BA2_1
		; r11, r12 out

check_colorBA2_1		; r11:@t->no_cero ,r12=NUM_FILAS y color
		mov		r12, #NUM_FILAS
		; si hubiera registros de sobra, poner la inst de abajo al principio
		mla		r11, r12, r5, r0	; r9=(MAX_NO_CERO*NUM_FILAS)+t ME HE SALTADO COLUMNAS
		mla		r11, r9, r5, r11	; nos movemos a la fila de la matriz no_cero necesaria (tienen max_cero comp)
		ldrb 	r4, [r11, r4]		; (t->no_ceros[fila]r11)[col(r4)]
		and 	r12, r4, #0x03		; and celda & 0x03
		cmp		r12, r3				; r7 == r3
		bne 	endwhile_1BA2_1				; si es igual-> exito
		; mirar si se pueden cargar aqui los deltas poniendole flags a add y cargandolo si r6 es 0
		add 	r6, r6, #1		; ++longitud
		add 	r9, r9, r7		; nueva_fila += delta_fila
		add 	r10, r10, r8	; nueva_columna += delta_columna
		b 		while_1BA2_1
endwhile_1BA2_1
;---------------------------------------------------------------- FIN BA2_1 -------------------------------------------------------------------
		ldmia SP!, {r9-r10}		;Recuperamos valores de registros y los guardamos en r2 y r3
		
		mov r10, r6
		cmp r10, #K_SIZE			;Comprobamos el resultado obtenido en buscar alineamiento
		movge r9, #1				;linea = TRUE
		bge finHL
		
		rsb r8, r8, #0				;-deltaF[i]
		rsb r7, r7, #0				;-deltaC[i]	
		stmdb SP!, {r9-r10}
		
		add r9, r1, r7
		add r10, r2, r8
		
;---------------------------------------------------------------- COMIENZO BA2_2 --------------------------------------------------------------------
		; r0=t, r1=fila, r2=columna, r3=color, r5=MAX_NO_CERO, r6=longitud, r7=TEMP:delta_fil, r8=TEMP:delta_col, r9=TEMP:nueva_fila, r10=TEMP:nueva_columna
		mov r5, #MAX_NO_CERO	
		mov	r6, #0			; longitud = 0
		
while_1BA2_2
		cmp		r9, #NUM_FILAS
		bhs		endwhile_1BA2_2
		cmp 	r10, #NUM_COLUMNAS
		bhs		endwhile_1BA2_2
		; r4=TEMP:col, r11=TEMP:&tablero->columna, r12=TEMP:valor tabl->[fila][col]
		mov 	r4, #0
		mla		r11, r9, r5, r0	; addr del vector COLUMNA(t->columnas[nueva_fila])
		
while_2BA2_2	
		cmp 	r4, r5			; if(col == MAX_NO_CERO)
		bhs		endwhile_1BA2_2		; return longitud
		ldrb 	r12, [r11, r4]	; r4 = (t->columnas[fila]r11)[col(r4)] no hay desplazamiento (uint8->1B)
		cmp 	r12, r10			; si son iguales ya esta encontrado
		;beq 	endwhile_2			
		addne 	r4, r4, #1		; col++ solo si son distintos
		bne		while_2BA2_2	; vuelve si son distintos
endwhile_2BA2_2
		; r11, r12 out

check_colorBA2_2		; r11:@t->no_cero ,r12=NUM_FILAS y color
		mov		r12, #NUM_FILAS
		; si hubiera registros de sobra, poner la inst de abajo al principio
		mla		r11, r12, r5, r0	; r9=(MAX_NO_CERO*NUM_FILAS)+t ME HE SALTADO COLUMNAS
		mla		r11, r9, r5, r11	; nos movemos a la fila de la matriz no_cero necesaria (tienen max_cero comp)
		ldrb 	r4, [r11, r4]		; (t->no_ceros[fila]r11)[col(r4)]
		and 	r12, r4, #0x03		; and celda & 0x03
		cmp		r12, r3				; r7 == r3
		bne 	endwhile_1BA2_2				; si es igual-> exito
		; mirar si se pueden cargar aqui los deltas poniendole flags a add y cargandolo si r6 es 0
		add 	r6, r6, #1		; ++longitud
		add 	r9, r9, r7		; nueva_fila += delta_fila
		add 	r10, r10, r8	; nueva_columna += delta_columna
		b 		while_1BA2_2
endwhile_1BA2_2
;---------------------------------------------------------------- FIN BA2_2 --------------------------------------------------------------------

		ldmia SP!, {r9-r10}
		
		add r10, r10, r6
		cmp r10, #K_SIZE
		movge r9, #1
		bge finHL			;Pq linea = TRUE
		
		
tercera_iter_buc		
		ldr r4, =delF			
		ldr r5, =delC
		ldrsb r7, [r4, #2]		
		ldrsb r8, [r5, #2]
		stmdb SP!, {r9-r10}		;Apilo EN pila regs que se van a soberescribir y no puedo obtener de memoria (no como r4 y r5 que es un ldr y ya)
		
		mov	r9, r1
		mov r10, r2

		;---------------------------------------------------------------- COMIENZO BA3_1 -------------------------------------------------------------------
		; r0=t, r1=fila, r2=columna, r3=color, r5=MAX_NO_CERO, r6=longitud, r7=TEMP:delta_fil, r8=TEMP:delta_col, r9=TEMP:nueva_fila, r10=TEMP:nueva_columna
		mov r5, #MAX_NO_CERO	
		mov	r6, #0			; longitud = 1
		
while_1BA3_1
		cmp		r9, #NUM_FILAS
		bhs		endwhile_1BA3_1
		cmp 	r10, #NUM_COLUMNAS
		bhs		endwhile_1BA3_1
		; r4=TEMP:col, r11=TEMP:&tablero->columna, r12=TEMP:valor tabl->[fila][col]
		mov 	r4, #0
		mla		r11, r9, r5, r0	; addr del vector COLUMNA(t->columnas[nueva_fila])
		
while_2BA3_1	
		cmp 	r4, r5			; if(col == MAX_NO_CERO)
		bhs		endwhile_1BA3_1		; return longitud
		ldrb 	r12, [r11, r4]	; r4 = (t->columnas[fila]r11)[col(r4)] no hay desplazamiento (uint8->1B)
		cmp 	r12, r10			; si son iguales ya esta encontrado
		;beq 	endwhile_2			
		addne 	r4, r4, #1		; col++ solo si son distintos
		bne		while_2BA3_1	; vuelve si son distintos
endwhile_2BA3_1
		; r11, r12 out

check_colorBA3_1		; r11:@t->no_cero ,r12=NUM_FILAS y color
		mov		r12, #NUM_FILAS
		; si hubiera registros de sobra, poner la inst de abajo al principio
		mla		r11, r12, r5, r0	; r9=(MAX_NO_CERO*NUM_FILAS)+t ME HE SALTADO COLUMNAS
		mla		r11, r9, r5, r11	; nos movemos a la fila de la matriz no_cero necesaria (tienen max_cero comp)
		ldrb 	r4, [r11, r4]		; (t->no_ceros[fila]r11)[col(r4)]
		and 	r12, r4, #0x03		; and celda & 0x03
		cmp		r12, r3				; r7 == r3
		bne 	endwhile_1BA3_1				; si es igual-> exito
		; mirar si se pueden cargar aqui los deltas poniendole flags a add y cargandolo si r6 es 0
		add 	r6, r6, #1		; ++longitud
		add 	r9, r9, r7		; nueva_fila += delta_fila
		add 	r10, r10, r8	; nueva_columna += delta_columna
		b 		while_1BA3_1
endwhile_1BA3_1
;---------------------------------------------------------------- FIN BA3_1 -------------------------------------------------------------------
		ldmia SP!, {r9-r10}		;Recuperamos valores de registros y los guardamos en r9 y r10
		
		mov r10, r6
		cmp r10, #K_SIZE		
		movge r9, #1				;linea = TRUE
		bge finHL
		
		rsb r8, r8, #0				;-deltaF[i]
		rsb r7, r7, #0				;-deltaC[i]	
		stmdb SP!, {r9-r10}
		
		add r9, r1, r7
		add r10, r2, r8
		
;---------------------------------------------------------------- COMIENZO BA3_2 --------------------------------------------------------------------
		; r0=t, r1=fila, r2=columna, r3=color, r5=MAX_NO_CERO, r6=longitud, r7=TEMP:delta_fil, r8=TEMP:delta_col, r9=TEMP:nueva_fila, r10=TEMP:nueva_columna
		mov r5, #MAX_NO_CERO	
		mov	r6, #0			; longitud = 1
		
while_1BA3_2
		cmp		r9, #NUM_FILAS
		bhs		endwhile_1BA3_2
		cmp 	r10, #NUM_COLUMNAS
		bhs		endwhile_1BA3_2
		; r4=TEMP:col, r11=TEMP:&tablero->columna, r12=TEMP:valor tabl->[fila][col]
		mov 	r4, #0
		mla		r11, r9, r5, r0	; addr del vector COLUMNA(t->columnas[nueva_fila])
		
while_2BA3_2	
		cmp 	r4, r5			; if(col == MAX_NO_CERO)
		bhs		endwhile_1BA3_2		; return longitud
		ldrb 	r12, [r11, r4]	; r4 = (t->columnas[fila]r11)[col(r4)] no hay desplazamiento (uint8->1B)
		cmp 	r12, r10			; si son iguales ya esta encontrado
		;beq 	endwhile_2			
		addne 	r4, r4, #1		; col++ solo si son distintos
		bne		while_2BA3_2	; vuelve si son distintos
endwhile_2BA3_2
		; r11, r12 out

check_colorBA3_2		; r11:@t->no_cero ,r12=NUM_FILAS y color
		mov		r12, #NUM_FILAS
		; si hubiera registros de sobra, poner la inst de abajo al principio
		mla		r11, r12, r5, r0	; r9=(MAX_NO_CERO*NUM_FILAS)+t ME HE SALTADO COLUMNAS
		mla		r11, r9, r5, r11	; nos movemos a la fila de la matriz no_cero necesaria (tienen max_cero comp)
		ldrb 	r4, [r11, r4]		; (t->no_ceros[fila]r11)[col(r4)]
		and 	r12, r4, #0x03		; and celda & 0x03
		cmp		r12, r3				; r7 == r3
		bne 	endwhile_1BA3_2				; si es igual-> exito
		; mirar si se pueden cargar aqui los deltas poniendole flags a add y cargandolo si r6 es 0
		add 	r6, r6, #1		; ++longitud
		add 	r9, r9, r7		; nueva_fila += delta_fila
		add 	r10, r10, r8	; nueva_columna += delta_columna
		b 		while_1BA3_2
endwhile_1BA3_2
;---------------------------------------------------------------- FIN BA3_2 --------------------------------------------------------------------

		ldmia SP!, {r9-r10}
		
		add r10, r10, r6
		cmp r10, #K_SIZE
		movge r9, #1
		bge finHL			;Pq linea = TRUE

		
cuarta_iter_buc		
		ldr r4, =delF			
		ldr r5, =delC
		ldrsb r7, [r4, #3]		
		ldrsb r8, [r5, #3]
		stmdb SP!, {r9-r10}		;Apilo EN pila regs que se van a soberescribir y no puedo obtener de memoria (no como r4 y r5 que es un ldr y ya)
		
		mov	r9, r1
		mov r10, r2

		;---------------------------------------------------------------- COMIENZO BA4_1 -------------------------------------------------------------------
		; r0=t, r1=fila, r2=columna, r3=color, r5=MAX_NO_CERO, r6=longitud, r7=TEMP:delta_fil, r8=TEMP:delta_col, r9=TEMP:nueva_fila, r10=TEMP:nueva_columna
		mov r5, #MAX_NO_CERO	
		mov	r6, #0			; longitud = 1
		
while_1BA4_1
		cmp		r9, #NUM_FILAS
		bhs		endwhile_1BA4_1
		cmp 	r10, #NUM_COLUMNAS
		bhs		endwhile_1BA4_1
		; r4=TEMP:col, r11=TEMP:&tablero->columna, r12=TEMP:valor tabl->[fila][col]
		mov 	r4, #0
		mla		r11, r9, r5, r0	; addr del vector COLUMNA(t->columnas[nueva_fila])
		
while_2BA4_1	
		cmp 	r4, r5			; if(col == MAX_NO_CERO)
		bhs		endwhile_1BA4_1		; return longitud
		ldrb 	r12, [r11, r4]	; r4 = (t->columnas[fila]r11)[col(r4)] no hay desplazamiento (uint8->1B)
		cmp 	r12, r10			; si son iguales ya esta encontrado
		;beq 	endwhile_2			
		addne 	r4, r4, #1		; col++ solo si son distintos
		bne		while_2BA4_1	; vuelve si son distintos
endwhile_2BA4_1
		; r11, r12 out

check_colorBA4_1		; r11:@t->no_cero ,r12=NUM_FILAS y color
		mov		r12, #NUM_FILAS
		; si hubiera registros de sobra, poner la inst de abajo al principio
		mla		r11, r12, r5, r0	; r9=(MAX_NO_CERO*NUM_FILAS)+t ME HE SALTADO COLUMNAS
		mla		r11, r9, r5, r11	; nos movemos a la fila de la matriz no_cero necesaria (tienen max_cero comp)
		ldrb 	r4, [r11, r4]		; (t->no_ceros[fila]r11)[col(r4)]
		and 	r12, r4, #0x03		; and celda & 0x03
		cmp		r12, r3				; r7 == r3
		bne 	endwhile_1BA4_1				; si es igual-> exito
		; mirar si se pueden cargar aqui los deltas poniendole flags a add y cargandolo si r6 es 0
		add 	r6, r6, #1		; ++longitud
		add 	r9, r9, r7		; nueva_fila += delta_fila
		add 	r10, r10, r8	; nueva_columna += delta_columna
		b 		while_1BA4_1
endwhile_1BA4_1
;---------------------------------------------------------------- FIN BA4_1 -------------------------------------------------------------------
		ldmia SP!, {r9-r10}		;Recuperamos valores de registros y los guardamos en r2 y r3
		
		mov r10, r6
		cmp r10, #K_SIZE		
		movge r9, #1				;linea = TRUE
		bge finHL
		
		rsb r8, r8, #0				;-deltaF[i]
		rsb r7, r7, #0				;-deltaC[i]	
		stmdb SP!, {r9-r10}
		
		add r9, r1, r7
		add r10, r2, r8
		
;---------------------------------------------------------------- COMIENZO BA4_2 --------------------------------------------------------------------
		; r0=t, r1=fila, r2=columna, r3=color, r5=MAX_NO_CERO, r6=longitud, r7=TEMP:delta_fil, r8=TEMP:delta_col, r9=TEMP:nueva_fila, r10=TEMP:nueva_columna
		mov r5, #MAX_NO_CERO	
		mov	r6, #0			; longitud = 1
		
while_1BA4_2
		cmp		r9, #NUM_FILAS
		bhs		endwhile_1BA4_2
		cmp 	r10, #NUM_COLUMNAS
		bhs		endwhile_1BA4_2
		; r4=TEMP:col, r11=TEMP:&tablero->columna, r12=TEMP:valor tabl->[fila][col]
		mov 	r4, #0
		mla		r11, r9, r5, r0	; addr del vector COLUMNA(t->columnas[nueva_fila])
		
while_2BA4_2	
		cmp 	r4, r5			; if(col == MAX_NO_CERO)
		bhs		endwhile_1BA4_2		; return longitud
		ldrb 	r12, [r11, r4]	; r4 = (t->columnas[fila]r11)[col(r4)] no hay desplazamiento (uint8->1B)
		cmp 	r12, r10			; si son iguales ya esta encontrado
		;beq 	endwhile_2			
		addne 	r4, r4, #1		; col++ solo si son distintos
		bne		while_2BA4_2	; vuelve si son distintos
endwhile_2BA4_2
		; r11, r12 out

check_colorBA4_2		; r11:@t->no_cero ,r12=NUM_FILAS y color
		mov		r12, #NUM_FILAS
		; si hubiera registros de sobra, poner la inst de abajo al principio
		mla		r11, r12, r5, r0	; r9=(MAX_NO_CERO*NUM_FILAS)+t ME HE SALTADO COLUMNAS
		mla		r11, r9, r5, r11	; nos movemos a la fila de la matriz no_cero necesaria (tienen max_cero comp)
		ldrb 	r4, [r11, r4]		; (t->no_ceros[fila]r11)[col(r4)]
		and 	r12, r4, #0x03		; and celda & 0x03
		cmp		r12, r3				; r7 == r3
		bne 	endwhile_1BA4_2				; si es igual-> exito
		; mirar si se pueden cargar aqui los deltas poniendole flags a add y cargandolo si r6 es 0
		add 	r6, r6, #1		; ++longitud
		add 	r9, r9, r7		; nueva_fila += delta_fila
		add 	r10, r10, r8	; nueva_columna += delta_columna
		b 		while_1BA4_2
endwhile_1BA4_2
;---------------------------------------------------------------- FIN BA4_2 --------------------------------------------------------------------

		ldmia SP!, {r9-r10}
		
		add r10, r10, r6
		cmp r10, #K_SIZE
		movge r9, #1
		
finHL	
		mov r0, r9				;lo que devolverá la funcion es este booleano
		ldmia SP, {r4-r10,FP,SP,PC}
		
	END