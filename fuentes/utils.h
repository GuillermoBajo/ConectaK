#include <inttypes.h>
#include <stddef.h>

#ifndef UTILS_H
#define UTILS_H

/**
 * @brief Convierte una cadena de caracteres en un número entero sin signo.
 * @param buffer_carga Cadena de caracteres a convertir.
 * @return El número entero sin signo resultante.
 */
uint32_t atoi(const char* buffer_carga);

/**
 * @brief Convierte un número entero en un arreglo de caracteres.
 * @param num Número entero a convertir.
 * @param charArray Arreglo de caracteres donde se almacenará el resultado.
 */
void intToCharArray(int num, char* charArray);

/**
 * @brief Convierte un número entero en una cadena de caracteres.
 * @param num Número entero a convertir.
 * @param str Cadena de caracteres donde se almacenará el resultado.
 */
void itoa(int num, char str[]);

/**
 * @brief Invierte una cadena de caracteres.
 * @param str Cadena de caracteres a invertir.
 * @param length Longitud de la cadena de caracteres.
 */
void reverse(char str[], int length);

/**
 * @brief Concatena dos cadenas de caracteres.
 * @param dest Cadena de caracteres de destino.
 * @param src Cadena de caracteres fuente.
 * @return Puntero a la cadena de caracteres de destino.
 */
char *strcat (char *dest, const char *src);

/**
 * @brief Copia una cadena de caracteres en otra.
 * @param dest Cadena de caracteres de destino.
 * @param source Cadena de caracteres fuente.
 */
void strcpy(char dest[], const char source[]);

/**
 * @brief Compara dos cadenas de caracteres.
 * @param s1 Primera cadena de caracteres a comparar.
 * @param s2 Segunda cadena de caracteres a comparar.
 * @return Un valor negativo si s1 es menor que s2, un valor positivo si s1 es mayor que s2, o 0 si son iguales.
 */
int strcmp(const char* s1, const char* s2);

/**
 * @brief Llena un bloque de memoria con un valor específico.
 * @param dst Puntero al bloque de memoria.
 * @param len Longitud del bloque de memoria.
 * @param val Valor a asignar a cada byte del bloque de memoria.
 */
void ms(char *dst, size_t len, char val);

#endif
