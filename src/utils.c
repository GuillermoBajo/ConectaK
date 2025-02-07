#include "utils.h"

uint32_t atoi(const char* buffer_carga){
	uint8_t i;
	uint32_t num = 0;
	for (i = 0; buffer_carga[i] != '\0'; ++i) {
		num = num * 10;
		if (buffer_carga[i] != '-') {
			 num += (buffer_carga[i] - '0');
		} 
	}
	return num;
}

void intToCharArray(int num, char* charArray) {
	int i;
	int len = 0;
	int cpy = num;
	while (cpy != 0) {
		cpy = cpy / 10;
		len++;
	}
	i = len-1;
	
	charArray[len] = '\0';
	if (num == 0) {
		charArray[i] = '0';
		return;
	}
	
	while (num != 0 && i >= 0) {
		charArray[i] = (num % 10) + '0';
		num = num / 10;
		i--;
	}
}

void itoa(int num, char str[]) {
	int i = 0;
	int sign = 1;

	if (num < 0) {
			sign = -1;
			num = -num;
	}
	do {
			str[i++] = num % 10 + '0';
			num = num / 10;
	} while (num != 0);

	if (sign == -1) {
			str[i++] = '-';
	}
	str[i] = '\0';
	reverse(str, i);
}

void reverse(char str[], int length) {
	int start = 0;
	int end = length - 1;
	while (start < end) {
		char temp = str[start];
		str[start] = str[end];
		str[end] = temp;
		start++;
		end--;
	}
}

char *strcat (char *dest, const char *src){
	const char *p;
	char *q;

	for (q = dest; *q != '\0'; q++)
		 ;
	
	for(p = src; *p != '\0'; p++, q++)
		 *q = *p;
	
	*q = '\0';

	return dest;
}

void strcpy(char dest[], const char source[]) {
	int i = 0;
	while ((dest[i] = source[i]) != '\0') {
			i++;
	} 
}

int strcmp(const char* s1, const char* s2)
{
	while(*s1 && (*s1 == *s2))
	{
			s1++;
			s2++;
	}
	return *(const unsigned char*)s1 - *(const unsigned char*)s2;
}

void ms(char *dst, size_t len, char val) {
	char *p;
	for (p = dst; p < dst + len; ++p) {
		*p = val;
	}
}

