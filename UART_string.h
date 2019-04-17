/*
 * UART_string.h
 *
 *  Created on: 15/04/2019
 *      Author: Lilia Lobato
 */

#ifndef UART_STRING_H_
#define UART_STRING_H_

//DIRECCIONES DE DATOS
typedef enum {
	STRING_UNO=1, STRING_DOS, STRING_TRES, STRING_CUATRO, STRING_CINCO
} UART_StringNumber;

//Regresa un string
uint8_t* UART_getString(UART_StringNumber stringNum);

//Cambia el valor de un string
void UART_setString(uint8_t* string, UART_StringNumber stringNum);

//Limpia el valor de un string
void UART_cleanString(UART_StringNumber stringNum);

//Limpia el valor de un todas las string
void UART_cleanAllString();

//Escribe las cadenas en pantalla
void UART_printString();

#endif /* UART_STRING_H_ */
