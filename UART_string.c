/*
 * UART_string.c
 *
 *  Created on: 15/04/2019
 *      Author: Lilia Lobato
 */

#include "I2C.h"
#include "UART_string.h"
#include "UART.h"

//Guarda las strings
uint8_t string1[30];
uint8_t string2[30];
uint8_t string3[30];
uint8_t string4[30];
uint8_t string5[30];

//Regresa un string
uint8_t* UART_getString(UART_StringNumber stringNum){
	switch (stringNum) {
		case STRING_UNO:
			return string1;
			break;
		case STRING_DOS:
			return string2;
			break;
		case STRING_TRES:
			return string3;
			break;
		case STRING_CUATRO:
			return string4;
			break;
		case STRING_CINCO:
			return string5;
			break;
		}
}

//Cambia el valor de un string
void UART_setString(uint8_t* string, UART_StringNumber stringNum) {
	*string++;
	switch (stringNum) {
	case STRING_UNO:
		for (uint8_t i = 0; i < 30; i++) {
			string1[i] = *string++;
		}
		break;
	case STRING_DOS:
		for (uint8_t i = 0; i < 30; i++) {
			string2[i] = *string++;
		}
		break;
	case STRING_TRES:
		for (uint8_t i = 0; i < 30; i++) {
			string3[i] = *string++;
		}
		break;
	case STRING_CUATRO:
		for (uint8_t i = 0; i < 30; i++) {
			string4[i] = *string++;
		}
		break;
	case STRING_CINCO:
		for (uint8_t i = 0; i < 30; i++) {
			string5[i] = *string++;
		}
		break;
	}
}

//Limpia el valor de un string
void UART_cleanString(UART_StringNumber stringNum) {
	switch (stringNum) {
	case STRING_UNO:
		for (uint8_t i = 0; i < 30; i++) {
			string1[i] = '\0';
		}
		break;
	case STRING_DOS:
		for (uint8_t i = 0; i < 30; i++) {
			string2[i] = '\0';
		}
		break;
	case STRING_TRES:
		for (uint8_t i = 0; i < 30; i++) {
			string3[i] = '\0';
		}
		break;
	case STRING_CUATRO:
		for (uint8_t i = 0; i < 30; i++) {
			string4[i] = '\0';
		}
		break;
	case STRING_CINCO:
		for (uint8_t i = 0; i < 30; i++) {
			string5[i] = '\0';
		}
		break;
	}

}

//Limpia el valor de un todas las string
void UART_cleanAllString() {
	UART_cleanString(STRING_UNO);
	UART_cleanString(STRING_DOS);
	UART_cleanString(STRING_TRES);
	UART_cleanString(STRING_CUATRO);
	UART_cleanString(STRING_CINCO);
}

//Escribe las cadenas en pantalla
void UART_printString(){
	UART_put_string(UART_0, "\033[11;10H");
	UART_put_string(UART_0, "1) ");
	UART_put_string(UART_0, UART_getString(STRING_UNO));

	UART_put_string(UART_0, "\033[12;10H");
	UART_put_string(UART_0, "2) ");
	UART_put_string(UART_0, UART_getString(STRING_DOS));

	UART_put_string(UART_0, "\033[13;10H");
	UART_put_string(UART_0, "3) ");
	UART_put_string(UART_0, UART_getString(STRING_TRES));

	UART_put_string(UART_0, "\033[14;10H");
	UART_put_string(UART_0, "4) ");
	UART_put_string(UART_0, UART_getString(STRING_CUATRO));

	UART_put_string(UART_0, "\033[15;10H");
	UART_put_string(UART_0, "5) ");
	UART_put_string(UART_0, UART_getString(STRING_CINCO));

	UART_put_string(UART_0, "\033[17;10H");
}
