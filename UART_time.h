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
#define DIAGONAL '/'
#define NADA '\0'
#define UNO '1'
#define DOS '2'
#define TRES '3'
#define CUATRO '4'
#define CINCO '5'
#define SEIS '6'
#define SIETE '7'
#define S 'S'
#define I 'I'
#define N 'N'
#define O 'O'

static uint8_t entry_counter;
static uint8_t entry[31];

//State Machine/////////////////////////////////////////////////////////////////////////////////////////////////////////////
typedef enum {
	TIME_MENU, TIME_CHANGE,DATE_CHANGE, TIME_ACTUAL, DATE_ACTUAL, MEMORY_WRITE, LEDS_TIME, LEDS_MEMORY, STORE_STRING
} time_status;

typedef enum {
	ENTER_1, ENTER_2, ENTER_3, ENTER_4, ENTER_5,ENTER_6, ENTER_7, ENTER_NULL, ENTER_TIME, ENTER_DATE,ENTER_SI, ENTER_NO,ENTER_STRING
} ASCII_enter;

typedef struct {
	time_status time;
	uint8_t next[13];
} State_t;

time_status UART_check_buzon(ASCII_enter user_entry, time_status uart_state);
uint8_t UART_isTimeCorrect();
uint8_t UART_isDateCorrect();

uint8_t UART_getEntryCounter();
uint8_t getEntryTime(RTCC_RegisterAdresses time);
void UART_setEntryCounter(uint8_t i);
void UART_setEntry();

void UART_print_number(RTCC_RegisterAdresses reg);
void UART_print(uint32_t num);
void UART_currentTime();
void UART_currentDate();
void setScreen ();
void UART_changeDate();
void UART_changeTime();

uint8_t int_to_BCD(uint32_t value);
uint8_t ASCII_to_int (uint8_t timeChar);
uint8_t isBisiesto();

#endif /* UART_TIME_H_ */
