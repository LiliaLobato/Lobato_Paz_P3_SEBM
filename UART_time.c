/*
 * UART_time.c
 *
 *  Created on: 10/04/2019
 *      Author: Lilia Lobato
 */

#include "NVIC.h"/**NVIC device driver*/
#include "UART.h"
#include <stdint.h>
#include "UART_time.h"
#include "reloj.h"

#define AUX_NUM "0"
#define NUM_CERO '0'

const State_t FSM_Moore[3] = { { TIME_MENU, { TIME_ACTUAL, TIME_CHANGE,
		TIME_MENU, TIME_MENU } },/* menu */
{ TIME_CHANGE, { TIME_CHANGE, TIME_CHANGE, TIME_CHANGE, TIME_MENU } },/* cambio de tiempo */
{ TIME_ACTUAL, { TIME_MENU, TIME_MENU, TIME_MENU, TIME_MENU } } /* tiempo actual */
};

extern uart_mail_box_t g_mail_box_uart_0;

uint8_t UART_getEntryCounter() {
	return entry_counter;
}

void UART_setEntryCounter(uint8_t i) {
	entry_counter = i;
}

void UART_setEntry() {
	for (uint8_t i = 0; i < 50; i++) {
		entry[i] = NADA;
	}
}

time_status UART_check_buzon(ASCII_enter user_entry, time_status uart_state) {
	if (g_mail_box_uart_0.flag) {
		if (ENTER != g_mail_box_uart_0.mailBox) {
			entry[entry_counter] = g_mail_box_uart_0.mailBox;
			UART_put_char(UART_0, g_mail_box_uart_0.mailBox);
			//printf("%s", entry[entry_counter]);
			entry_counter++;

			//Guarda en entry hasta que encuentra un ENTER
		} else {
			//si está vacio, NULL_ENTER
			if (NADA == entry[1]) {
				user_entry = ENTER_NULL;
			} else if (DOSPUNTOS == entry[3] & DOSPUNTOS == entry[6]
					& NADA == entry[9]) { //revisa si tiene formato de hora(2 :) TIMER_ENTER
					//valida
				if (UART_isTimeCorrect()) {
					user_entry = ENTER_TIME;
					//Cambiar tiempo en chip
					if (12 >= getEntryTime(HOURS)) { //los valores de entry están en ascii no en decimal!!!!! por eso da cosas raras
						RTCC_ChangeValue(
								int_to_BCD(getEntryTime(HOURS))
										| 0x40 & ~(0x20), HOURS); //cambio en horas
					} else {
						RTCC_ChangeValue(
								int_to_BCD(getEntryTime(HOURS) - 12) | 0x40
										| 0X20, HOURS); //cambio en horas
					}
					RTCC_ChangeValue(int_to_BCD(getEntryTime(MINUTES)), MINUTES); //cambio en minutos
					RTCC_ChangeValue(int_to_BCD(getEntryTime(SECONDS)) | 0x80,SECONDS); //cambio en segundos
					//RTCC_setGlobalVariable
					RTCC_setGlobalVariable(HOURS, getEntryTime(HOURS)); //Cambio registro horas
					RTCC_setGlobalVariable(MINUTES, getEntryTime(MINUTES)); //Cambio registro minutos
					RTCC_setGlobalVariable(SECONDS, getEntryTime(SECONDS)); //Cambio registro segundos
				}
				//else ENTER_NULL
			} else if ((UNO == entry[1] | DOS == entry[1])
					& (NADA == entry[2])) { //revisa para menu, revisa que no tenga nada extra, MENU
				if (UNO == entry[1]) {
					user_entry = ENTER_1;
				} else {
					user_entry = ENTER_2;
				}
			} else { //lo demas se va a NULL_ENTER
				user_entry = ENTER_NULL;
			}

			uart_state = FSM_Moore[uart_state].next[user_entry];

			//Limpia pantalla
			/* VT100 command for text in red and background in cyan*/
			UART_put_string(UART_0, "\033[0;35;43m");
			/*VT100 command for clearing the screen*/
			UART_put_string(UART_0, "\033[2J");
			/* VT100 command for text in red and background in green*/
			UART_put_string(UART_0, "\033[0;32;41m");

			//limpia array
			UART_setEntry();
			//limpiar contador
			entry_counter = 0;
		}
	}
	//clear the reception flag
	g_mail_box_uart_0.flag = 0;
	return uart_state;

}

uint8_t UART_isTimeCorrect() {
	//Revisa que la hora ingresada sea correcta
	if (entry[1] <= 50 & entry[1] >= 48) { //0:2
		if (entry[2] <= 57 & entry[2] >= 48) { //0:9
			if (entry[4] <= 53 & entry[4] >= 48) { //0:5
				if (entry[5] <= 57 & entry[5] >= 48) { //0:9
					if (entry[7] <= 53 & entry[7] >= 48) { //0:6
						if (entry[8] <= 57 & entry[8] >= 48) { //0:9
							return TRUE;
						}
					}
				}
			}
		}
	} else {
		return FALSE;
	}
}

void UART_currentTime() {
	RTCC_ReadDateTimeFull();

	UART_put_string(UART_0, "\033[11;10H");

	if(FALSE == RTCC_getGlobalVariable(AMPM)& 0X40>>6) {
		UART_print_number(HOURS);
	} else {
		UART_print_number(HOURS);
		UART_put_string(UART_0, "\033[11;10H");
		if(RTCC_getGlobalVariable(HOURS)/10){
			UART_put_string(UART_0, "1");
		} else {
		UART_put_string(UART_0, "0");
		}
		UART_put_string(UART_0, "\033[11;12H");
	}
	UART_put_string(UART_0, ":");
	UART_print_number(MINUTES);
	UART_put_string(UART_0, ":");
	UART_print_number(SECONDS);
	if (FALSE == RTCC_getGlobalVariable(AMPM)) {
		UART_put_string(UART_0, " AM");
	} else {
		UART_put_string(UART_0, " PM");
	}
}

void UART_print_number(RTCC_RegisterAdresses reg) {
	uint32_t num = RTCC_getGlobalVariable(reg);
	uint32_t numH = num / 10; //decenas de la unidad de tiempo
	uint32_t numL = num - (num / 10) * 10;
	UART_print(numH); //Envia decenas
	UART_print(numL); // envia unidades
}

void UART_print(uint32_t num) {
	switch (num) {
	case 0:
		UART_put_string(UART_0, "0");
		break;
	case 1:
		UART_put_string(UART_0, "1");
		break;
	case 2:
		UART_put_string(UART_0, "2");
		break;
	case 3:
		UART_put_string(UART_0, "3");
		break;
	case 4:
		UART_put_string(UART_0, "4");
		break;
	case 5:
		UART_put_string(UART_0, "5");
		break;
	case 6:
		UART_put_string(UART_0, "6");
		break;
	case 7:
		UART_put_string(UART_0, "7");
		break;
	case 8:
		UART_put_string(UART_0, "8");
		break;
	case 9:
		UART_put_string(UART_0, "9");
		break;
	default:
		UART_put_string(UART_0, "6");
		break;

	}
}

uint8_t int_to_BCD(uint32_t value) {
	return ((value / 10 * 16) + (value % 10));
}

uint8_t getEntryTime(RTCC_RegisterAdresses time) {
	switch (time) {
	case HOURS:
		return ASCII_to_int(entry[1]) * 10 + ASCII_to_int(entry[2]);
		break;
	case MINUTES:
		return ASCII_to_int(entry[4]) * 10 + ASCII_to_int(entry[5]);
		break;
	case SECONDS:
		return ASCII_to_int(entry[7]) * 10 + ASCII_to_int(entry[8]);
		break;
	}
}

uint8_t ASCII_to_int(uint8_t timeChar) {
	switch (timeChar) {
	case '0':
		return 0;
		break;
	case '1':
		return 1;
		break;
	case '2':
		return 2;
		break;
	case '3':
		return 3;
		break;
	case '4':
		return 4;
		break;
	case '5':
		return 5;
		break;
	case '6':
		return 6;
		break;
	case '7':
		return 7;
		break;
	case '8':
		return 8;
		break;
	case '9':
		return 9;
		break;
	}
}
