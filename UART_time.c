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
#include "Matrcial_LEDs.h"
#include "UART_string.h"
#include "RAM.h"

#define AUX_NUM "0"
#define NUM_CERO '0'

const State_t FSM_Moore[9] = { { TIME_MENU, { TIME_CHANGE, DATE_CHANGE,
		TIME_ACTUAL, DATE_ACTUAL, MEMORY_WRITE, LEDS_TIME, LEDS_MEMORY,
		TIME_MENU, TIME_MENU, TIME_MENU, TIME_MENU, TIME_MENU, TIME_MENU } },/* menu */
{ TIME_CHANGE, { TIME_CHANGE, TIME_CHANGE, TIME_CHANGE, TIME_CHANGE,
		TIME_CHANGE, TIME_CHANGE, TIME_CHANGE, TIME_CHANGE, TIME_MENU,
		TIME_CHANGE, TIME_CHANGE, TIME_CHANGE, TIME_CHANGE } },/* cambio timepo */
{ DATE_CHANGE, { DATE_CHANGE, DATE_CHANGE, DATE_CHANGE, DATE_CHANGE,
		DATE_CHANGE, DATE_CHANGE, DATE_CHANGE, DATE_CHANGE, DATE_CHANGE,
		TIME_MENU, DATE_CHANGE, DATE_CHANGE, DATE_CHANGE } },/* cambio fecha */
{ TIME_ACTUAL, { TIME_MENU, TIME_MENU, TIME_MENU, TIME_MENU, TIME_MENU,
		TIME_MENU, TIME_MENU, TIME_MENU, TIME_MENU, TIME_MENU, TIME_MENU,
		TIME_MENU, TIME_MENU } },/* tiempo actual */
{ DATE_ACTUAL, { TIME_MENU, TIME_MENU, TIME_MENU, TIME_MENU, TIME_MENU,
		TIME_MENU, TIME_MENU, TIME_MENU, TIME_MENU, TIME_MENU, TIME_MENU,
		TIME_MENU, TIME_MENU } },/* fecha actual */
{ MEMORY_WRITE, { STORE_STRING, STORE_STRING, STORE_STRING, STORE_STRING,
		STORE_STRING, MEMORY_WRITE, MEMORY_WRITE, MEMORY_WRITE, MEMORY_WRITE,
		MEMORY_WRITE, MEMORY_WRITE, MEMORY_WRITE, MEMORY_WRITE } },/* escribir en memoria */
{ LEDS_TIME, { LEDS_TIME, LEDS_TIME, LEDS_TIME, LEDS_TIME, LEDS_TIME, LEDS_TIME,
		LEDS_TIME, LEDS_TIME, LEDS_TIME, LEDS_TIME, TIME_MENU, TIME_MENU,
		LEDS_TIME } },/* tiempo en display */
{ LEDS_MEMORY, { TIME_MENU, TIME_MENU, TIME_MENU, TIME_MENU, TIME_MENU,
		LEDS_MEMORY, LEDS_MEMORY, LEDS_MEMORY, LEDS_MEMORY, LEDS_MEMORY,
		LEDS_MEMORY, LEDS_MEMORY, LEDS_MEMORY } },/* string en display */
{ STORE_STRING, { TIME_MENU, TIME_MENU, TIME_MENU, TIME_MENU, TIME_MENU,
		TIME_MENU, TIME_MENU, STORE_STRING, TIME_MENU, TIME_MENU, TIME_MENU,
		TIME_MENU, TIME_MENU } }/* guardar srting */

};

extern uart_mail_box_t g_mail_box_uart_0;
UART_StringNumber stringNum;

uint8_t UART_getEntryCounter() {
	return entry_counter;
}

void UART_setEntryCounter(uint8_t i) {
	entry_counter = i;
}

void UART_setEntry() {
	for (uint8_t i = 0; i < 31; i++) {
		entry[i] = NADA;
	}
}

//Cambio en la maquina de estados
time_status UART_check_buzon(ASCII_enter user_entry, time_status uart_state) {
	//-----------------------GUARDA LO QUE SE ENCUENTRA EN PANTALLA
	if (g_mail_box_uart_0.flag) {
		if (ENTER != g_mail_box_uart_0.mailBox) {
			entry[entry_counter] = g_mail_box_uart_0.mailBox;
			UART_put_char(UART_0, g_mail_box_uart_0.mailBox);
			entry_counter++;
			//Guarda en entry hasta que encuentra un ENTER
		} else {
			//-----------------------SE DA UN CAMBIO EN LA MAQUINA DE ESTADOS
			//si está vacio, ENTER_NULL
			if (NADA == entry[1]) {
				user_entry = ENTER_NULL;
			} else if (DOSPUNTOS == entry[3] & DOSPUNTOS == entry[6]
					& NADA == entry[9]) {
				//revisa si tiene formato de hora(2 :) ENTER_TIME
				if (UART_isTimeCorrect()) {
					user_entry = ENTER_TIME;
					UART_changeTime(); //Cambiar tiempo en chip
				}
			} else if (DIAGONAL == entry[3] & DIAGONAL == entry[6]
					& NADA == entry[11]) {
				//Revisa si tiene formato de fecha (2 / ) ENTER_DATE
				if (UART_isDateCorrect()) {
					user_entry = ENTER_DATE;
					UART_changeDate(); //Cambiar tiempo en chip
				}
			} else if (S == entry[1] & I == entry[2] & NADA == entry[3]) {
				//revisa si es un SI ENTER_SI
				user_entry = ENTER_SI;
				Matricial_LEDs_Modo_cadena_callback_off();
				Matricial_LEDs_clear();
				Matricial_LEDs_Modo_Hora_init();
				Matricial_LEDs_modo_hora();

			} else if (N == entry[1] & O == entry[2] & NADA == entry[3]) {
				//revisa si es un NO ENTER_NO
				user_entry = ENTER_NO;
				Matricial_LEDs_Modo_cadena_callback_off();
				Matricial_LEDs_Modo_hora_callback_off();
				Matricial_LEDs_clear();
			}
			//revisa si uno de los números tecleados es una opción del menú, ENTER_#
			else if ((UNO == entry[1] | DOS == entry[1] | TRES == entry[1]
					| CUATRO == entry[1] | CINCO == entry[1] | SEIS == entry[1]
					| SIETE == entry[1]) & (NADA == entry[2])) { //revisa para menu, revisa que no tenga nada extra, MENU
				switch (entry[1]) {
				case UNO:
					user_entry = ENTER_1;
					//Guarda la opcino de string a guardar
					if (MEMORY_WRITE == uart_state) {
						stringNum = STRING_UNO;
					}
					//Manda la cadena correcta al display
					if (LEDS_MEMORY == uart_state) {
						Matricial_LEDs_Modo_hora_callback_off();
						Matricial_LEDs_clear_gString();
						if (RAM_error_cmp()) {
							Matricial_LEDs_set_string(
									RAM_read_string(STRING_UNO));
							//UART_getString(STRING_UNO);
						} else {
							Matricial_LEDs_set_string("DESCONECTADO");
						}
						Matricial_LEDs_Modo_cadena_init();
					}
					break;
				case DOS:
					user_entry = ENTER_2;
					if (MEMORY_WRITE == uart_state) {
						stringNum = STRING_DOS;
					}
					//Manda la cadena correcta al display
					if (LEDS_MEMORY == uart_state) {
						Matricial_LEDs_Modo_hora_callback_off();
						Matricial_LEDs_clear_gString();
						if (RAM_error_cmp()) {
							Matricial_LEDs_set_string(
									RAM_read_string(STRING_DOS));
						} else {
							Matricial_LEDs_set_string("DESCONECTADO");
						}
						Matricial_LEDs_Modo_cadena_init();
						//UART_getString(STRING_DOS);
					}
					break;
				case TRES:
					user_entry = ENTER_3;
					if (MEMORY_WRITE == uart_state) {
						stringNum = STRING_TRES;
					}
					//Manda la cadena correcta al display
					if (LEDS_MEMORY == uart_state) {
						Matricial_LEDs_Modo_hora_callback_off();
						Matricial_LEDs_clear_gString();
						if (RAM_error_cmp()) {
							Matricial_LEDs_set_string(
									RAM_read_string(STRING_TRES));
						} else {
							Matricial_LEDs_set_string("DESCONECTADO");
						}
						Matricial_LEDs_Modo_cadena_init();
						//UART_getString(STRING_TRES);
					}
					break;
				case CUATRO:
					user_entry = ENTER_4;
					if (MEMORY_WRITE == uart_state) {
						stringNum = STRING_CUATRO;
					}
					//Manda la cadena correcta al display
					if (LEDS_MEMORY == uart_state) {
						Matricial_LEDs_Modo_hora_callback_off();
						Matricial_LEDs_clear_gString();
						if (RAM_error_cmp()) {
							Matricial_LEDs_set_string(
									RAM_read_string(STRING_CUATRO));
						} else {
							Matricial_LEDs_set_string("DESCONECTADO");
						}
						Matricial_LEDs_Modo_cadena_init();
						//UART_getString(STRING_CUATRO);
					}
					break;
				case CINCO:
					user_entry = ENTER_5;
					if (MEMORY_WRITE == uart_state) {
						stringNum = STRING_CINCO;
					}
					//Manda la cadena correcta al display
					if (LEDS_MEMORY == uart_state) {
						Matricial_LEDs_Modo_hora_callback_off();
						Matricial_LEDs_clear_gString();
						if (RAM_error_cmp()) {
							Matricial_LEDs_set_string(
									RAM_read_string(STRING_CINCO));
						} else {
							Matricial_LEDs_set_string("DESCONECTADO");
						}
						Matricial_LEDs_Modo_cadena_init();
						//UART_getString(STRING_CINCO);
					}
					break;
				case SEIS:
					user_entry = ENTER_6;
					break;
				case SIETE:
					user_entry = ENTER_7;
					break;
				}
			} else { //lo demas se va a ENTER_NULL
				user_entry = ENTER_STRING;
				//SOLO CAMBIAR CUANDO SE ESTE EN STORE_STRING
				if (STORE_STRING == uart_state) {
					if (RAM_error_cmp()) {
						UART_setString(entry, stringNum);
						RAM_write(stringNum, UART_getString(stringNum));
					}
				}
			}

			uart_state = FSM_Moore[uart_state].next[user_entry];

			//limpia pantalla
			setScreen();
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
					if (entry[7] <= 53 & entry[7] >= 48) { //0:5
						if (entry[8] <= 57 & entry[8] >= 48) { //0:9
							if (24
									<= (ASCII_to_int(entry[1]) * 10
											+ ASCII_to_int(entry[2]))) {
								return FALSE;
							} else {
								return TRUE;
							}
						}
					}
				}
			}
		}
	} else {
		return FALSE;
	}
}

uint8_t UART_isDateCorrect() {
	uint8_t mes = ASCII_to_int(entry[4]) * 10 + ASCII_to_int(entry[5]);
	//Revisa que la hora ingresada sea correcta
	if ((48 == entry[1] & 48 == entry[2]) | (48 == entry[4] & 48 == entry[5])) {
		return FALSE;
	}
	if (entry[1] <= 51 & entry[1] >= 48) { //0:3
		if (entry[2] <= 57 & entry[2] >= 48) { //0:9
			if (entry[4] <= 49 & entry[4] >= 48) { //0:1
				if (entry[5] <= 57 & entry[5] >= 48) { //0:9
					if (entry[7] <= 50 & entry[7] >= 48) { //0:2
						if (entry[8] <= 57 & entry[8] >= 48) { //0:9
							if (entry[9] <= 57 & entry[9] >= 48) { //0:9
								if (entry[10] <= 57 & entry[10] >= 48) { //0:9
									if (2399
											< (ASCII_to_int(entry[7]) * 1000
													+ ASCII_to_int(entry[8])
															* 100
													+ ASCII_to_int(entry[9])
															* 10
													+ ASCII_to_int(entry[10]))) {
										return FALSE;
									} //VALIDA LOS AÑOS
									if (2 == mes) { //VALIDA LOS MESES
										if (FALSE == isBisiesto()) {
											if (29
													< (ASCII_to_int(entry[1])
															* 10
															+ ASCII_to_int(
																	entry[2]))) {
												return FALSE;
											} else {
												return TRUE;
											}
										} else if (28
												< (ASCII_to_int(entry[1]) * 10
														+ ASCII_to_int(entry[2]))) {
											return FALSE;
										} else {
											return TRUE;
										}
									} else if ((7 >= mes & FALSE != mes % 2)
											| (8 <= mes & FALSE == mes % 2)) { //valida mese de 31 dias
										if (31
												< (ASCII_to_int(entry[1]) * 10
														+ ASCII_to_int(entry[2]))) {
											return FALSE;
										} else {
											return TRUE;
										}
									} else if ((6 >= mes & FALSE == mes % 2)
											| (9 <= mes & FALSE != mes % 2)) { //valida meses de 30 dias
										if (30
												< (ASCII_to_int(entry[1]) * 10
														+ ASCII_to_int(entry[2]))) {
											return FALSE;
										} else {
											return TRUE;
										}
									}
								}
							}
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
//Lee el tiempo actual del chip
	RTCC_ReadDateTimeFull();
	UART_put_string(UART_0, "\033[11;10H");
//Imprime en formato AM/PM las horas
	UART_print_number(HOURS);
//imprime los minutos y segundos
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

void UART_currentDate() {
//Lee el tiempo actual del chip
	RTCC_ReadDateTimeFull();
	UART_put_string(UART_0, "\033[11;10H");
	UART_print_number(DAYS);
	UART_put_string(UART_0, "/");
	UART_print_number(MONTH);
	UART_put_string(UART_0, "/");
	UART_print_number(YEARS);
}

void UART_print_number(RTCC_RegisterAdresses reg) {
	uint32_t num = RTCC_getGlobalVariable(reg);
	uint32_t numH = num / 10; //decenas de la unidad de tiempo
	uint32_t numL = num - (num / 10) * 10;
	UART_print(numH); //Envia decenas
	UART_print(numL); // envia unidades
}

void UART_print(uint32_t num) { //convierte de número a string
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

uint8_t int_to_BCD(uint32_t value) { //convierte de int a BDC para poder guardar en el RTCC
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
	case YEARS:
		return (ASCII_to_int(entry[9]) * 10 + ASCII_to_int(entry[10]));
		break;
	case MONTH:
		return ASCII_to_int(entry[4]) * 10 + ASCII_to_int(entry[5]);
		break;
	case DAYS:
		return ASCII_to_int(entry[1]) * 10 + ASCII_to_int(entry[2]);
		break;
	}
}

uint8_t ASCII_to_int(uint8_t timeChar) { //convierte de ASCCI a int
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

void setScreen() { //Limpia la pantalla
	/**The following sentences send strings to PC using the UART_put_string function. Also, the string
	 * is coded with terminal code*/
	/** VT100 command for text in red and background in cyan*/
	UART_put_string(UART_0, "\033[0;30;47m");
	/*VT100 command for clearing the screen*/
	UART_put_string(UART_0, "\033[2J");
	/** VT100 command for text in red and background in green*/
	UART_put_string(UART_0, "\033[0;30;47m");
}

void UART_changeTime() { //cambia el tiempo en el RTCC y las variables globales
	if (12 > getEntryTime(HOURS)) { //guarda dependiendo si es 12 o 24
		if (FALSE == getEntryTime(HOURS)) {
			RTCC_ChangeValue(int_to_BCD(12) | 0x40 & ~(0x20), HOURS);
		} else {
			RTCC_ChangeValue(int_to_BCD(getEntryTime(HOURS)) | 0x40 & ~(0x20),
					HOURS); //cambio en horas
		}
	} else {
		if (12 == getEntryTime(HOURS)) {
			RTCC_ChangeValue(int_to_BCD(getEntryTime(HOURS)) | 0x40 | 0X20,
					HOURS);
		} else {
			RTCC_ChangeValue(int_to_BCD(getEntryTime(HOURS) - 12) | 0x40 | 0X20,
					HOURS); //cambio en horas
		}
	}
	RTCC_ChangeValue(int_to_BCD(getEntryTime(MINUTES)), MINUTES); //cambio en minutos
	RTCC_ChangeValue(int_to_BCD(getEntryTime(SECONDS)) | 0x80, SECONDS); //cambio en segundos
//RTCC_setGlobalVariable
	if (FALSE == getEntryTime(HOURS)) {
		RTCC_setGlobalVariable(HOURS, 12);
	} else {
		RTCC_setGlobalVariable(HOURS, getEntryTime(HOURS)); //Cambio registro horas
	}
	RTCC_setGlobalVariable(MINUTES, getEntryTime(MINUTES)); //Cambio registro minutos
	RTCC_setGlobalVariable(SECONDS, getEntryTime(SECONDS)); //Cambio registro segundos
}

void UART_changeDate() { //cambia la fecha en el RTCC y las variables globales

	RTCC_ChangeValue(int_to_BCD(getEntryTime(YEARS)), YEARS); //cambio en anios
	RTCC_ChangeValue(int_to_BCD(getEntryTime(MONTH)), MONTH); //cambio en meses
	RTCC_ChangeValue(int_to_BCD(getEntryTime(DAYS)), DAYS); //cambio en dias
//RTCC_setGlobalVariable
	RTCC_setGlobalVariable(YEARS, getEntryTime(YEARS)); //Cambio registro anios
	RTCC_setGlobalVariable(MONTH, getEntryTime(MONTH)); //Cambio registro meses
	RTCC_setGlobalVariable(DAYS, getEntryTime(DAYS)); //Cambio registro dias
}

uint8_t isBisiesto() {
	uint16_t anio = (ASCII_to_int(entry[7]) * 1000
			+ ASCII_to_int(entry[8]) * 100 + ASCII_to_int(entry[9]) * 10
			+ ASCII_to_int(entry[10]));
	if (anio % 4 == 0 && anio % 100 != 0 || anio % 400 == 0) {
		return FALSE;
	} else {
		return TRUE;
	}
}

void UART_initRTCC() {
	RTCC_ChangeValue(0x80, SECONDS); //enciende el bit 7 de los segundos
	RTCC_ChangeValue(0x52, HOURS);
	RTCC_setGlobalVariable(HOURS, 12);
	RTCC_ChangeValue(0x65, SRAM); //Guarda una A en 0x20 para comprobar que esté conectado el chip
}
