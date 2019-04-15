/*
 * UART_time.h
 *
 *  Created on: 10/04/2019
 *      Author: Lilia Lobato
 */

#ifndef UART_TIME_H_
#define UART_TIME_H_

#include "reloj.h"

#define ENTER '\r'
#define DOSPUNTOS ':'
#define NADA '\n'
#define UNO '1'
#define DOS '2'

static uint8_t entry_counter;
static uint8_t entry[50];

//State Machine/////////////////////////////////////////////////////////////////////////////////////////////////////////////
typedef enum {
	TIME_MENU, TIME_CHANGE, TIME_ACTUAL
} time_status;

typedef enum {
	ENTER_1, ENTER_2, ENTER_NULL, ENTER_TIME
} ASCII_enter;

typedef struct {
	time_status time;
	uint8_t next[4];
} State_t;



time_status UART_check_buzon(ASCII_enter user_entry, time_status uart_state);
uint8_t UART_isTimeCorrect();

uint8_t UART_getEntryCounter();
void UART_setEntryCounter(uint8_t i);
void UART_setEntry();
void UART_print_number(RTCC_RegisterAdresses reg);
void UART_print(uint32_t num);
void UART_currentTime();
uint8_t int_to_BCD(uint32_t value);

uint8_t getEntryTime(RTCC_RegisterAdresses time);
uint8_t ASCII_to_int (uint8_t timeChar);


#endif /* UART_TIME_H_ */
