/*
 * Matricial_LEDs.c
 *
 *  Created on: 19/04/2019
 *      Author: best buy
 */

#include "Delay.h"
#include "Matrcial_LEDs.h"

#define HT16K33_WRITE_ADDRESS 0xE0 //PHYSICAL HT16K33 ADDRESS

//Estado del reloj
uint8_t estadoRTCC = CONECTADO;

uint64_t g_character1 = 0;
uint64_t g_character2 = 0;
uint64_t g_next_char = 0;
uint64_t g_string[30];
uint8_t g_cont = 0;

void Matricial_LEDs_init(void) {

	delay(DELAY);

	I2C_start(I2C_0); //it configures the I2C in trasnmitting mode and generates the start

	I2C_write_byte(I2C_0, HT16K33_WRITE_ADDRESS); //Escribe la direccion de la matriz de LEDs
	I2C_wait(I2C_0); //Checking if the I2C module is busy by cheking the busy flag
	I2C_get_ack(I2C_0); //Waiting for acknowledge

	I2C_write_byte(I2C_0, 0x21); //Escribe secuencia de inicializacion
	I2C_wait(I2C_0); //Checking if the I2C module is busy by cheking the busy flag
	I2C_get_ack(I2C_0); //Waiting for acknowledge

	I2C_stop(I2C_0);

	I2C_start(I2C_0); //it configures the I2C in trasnmitting mode and generates the start

	I2C_write_byte(I2C_0, HT16K33_WRITE_ADDRESS); //Escribe la direccion de la matriz de LEDs
	I2C_wait(I2C_0); //Checking if the I2C module is busy by cheking the busy flag
	I2C_get_ack(I2C_0); //Waiting for acknowledge

	I2C_write_byte(I2C_0, 0x81); //Escribe secuencia de inicializacion
	I2C_wait(I2C_0); //Checking if the I2C module is busy by cheking the busy flag
	I2C_get_ack(I2C_0); //Waiting for acknowledge

	I2C_stop(I2C_0);

	I2C_start(I2C_0); //it configures the I2C in trasnmitting mode and generates the start

	I2C_write_byte(I2C_0, HT16K33_WRITE_ADDRESS); //Escribe la direccion de la matriz de LEDs
	I2C_wait(I2C_0); //Checking if the I2C module is busy by cheking the busy flag
	I2C_get_ack(I2C_0); //Waiting for acknowledge

	I2C_write_byte(I2C_0, 0xEF); //Escribe secuencia de inicializacion
	I2C_wait(I2C_0); //Checking if the I2C module is busy by cheking the busy flag
	I2C_get_ack(I2C_0); //Waiting for acknowledge

	I2C_stop(I2C_0);

	uint8_t i;
	for (i = 0; i < 30; i++) {
		g_string[i] = '\0'; //inicializamos la cadena en 0
	}

}

void Matricial_LEDs_test(void) {
	delay(DELAY);

	I2C_start(I2C_0); //it configures the I2C in trasnmitting mode and generates the start

	I2C_write_byte(I2C_0, HT16K33_WRITE_ADDRESS); //Escribe la direccion de la matriz de LEDs
	I2C_wait(I2C_0); //Checking if the I2C module is busy by cheking the busy flag
	I2C_get_ack(I2C_0); //Waiting for acknowledge

	I2C_write_byte(I2C_0, 0x00); //Escribe secuencia de inicializacion
	I2C_wait(I2C_0); //Checking if the I2C module is busy by cheking the busy flag
	I2C_get_ack(I2C_0); //Waiting for acknowledge

	I2C_write_byte(I2C_0, 0x81); //Escribe secuencia de inicializacion
	I2C_wait(I2C_0); //Checking if the I2C module is busy by cheking the busy flag
	I2C_get_ack(I2C_0); //Waiting for acknowledge

	I2C_write_byte(I2C_0, 0x81); //Escribe secuencia de inicializacion
	I2C_wait(I2C_0); //Checking if the I2C module is busy by cheking the busy flag
	I2C_get_ack(I2C_0); //Waiting for acknowledge

	I2C_write_byte(I2C_0, 0x70); //Escribe secuencia de inicializacion
	I2C_wait(I2C_0); //Checking if the I2C module is busy by cheking the busy flag
	I2C_get_ack(I2C_0); //Waiting for acknowledge

	I2C_write_byte(I2C_0, 0x70); //Escribe secuencia de inicializacion
	I2C_wait(I2C_0); //Checking if the I2C module is busy by cheking the busy flag
	I2C_get_ack(I2C_0); //Waiting for acknowledge

	I2C_write_byte(I2C_0, 0x60); //Escribe secuencia de inicializacion
	I2C_wait(I2C_0); //Checking if the I2C module is busy by cheking the busy flag
	I2C_get_ack(I2C_0); //Waiting for acknowledge

	I2C_write_byte(I2C_0, 0x68); //Escribe secuencia de inicializacion
	I2C_wait(I2C_0); //Checking if the I2C module is busy by cheking the busy flag
	I2C_get_ack(I2C_0); //Waiting for acknowledge

	I2C_write_byte(I2C_0, 0x50); //Escribe secuencia de inicializacion
	I2C_wait(I2C_0); //Checking if the I2C module is busy by cheking the busy flag
	I2C_get_ack(I2C_0); //Waiting for acknowledge

	I2C_write_byte(I2C_0, 0x05); //Escribe secuencia de inicializacion
	I2C_wait(I2C_0); //Checking if the I2C module is busy by cheking the busy flag
	I2C_get_ack(I2C_0); //Waiting for acknowledge

	I2C_write_byte(I2C_0, 0x40); //Escribe secuencia de inicializacion
	I2C_wait(I2C_0); //Checking if the I2C module is busy by cheking the busy flag
	I2C_get_ack(I2C_0); //Waiting for acknowledge

	I2C_write_byte(I2C_0, 0x04); //Escribe secuencia de inicializacion
	I2C_wait(I2C_0); //Checking if the I2C module is busy by cheking the busy flag
	I2C_get_ack(I2C_0); //Waiting for acknowledge

	I2C_write_byte(I2C_0, 0x30); //Escribe secuencia de inicializacion
	I2C_wait(I2C_0); //Checking if the I2C module is busy by cheking the busy flag
	I2C_get_ack(I2C_0); //Waiting for acknowledge

	I2C_write_byte(I2C_0, 0x15); //Escribe secuencia de inicializacion
	I2C_wait(I2C_0); //Checking if the I2C module is busy by cheking the busy flag
	I2C_get_ack(I2C_0); //Waiting for acknowledge

	I2C_write_byte(I2C_0, 0x2F); //Escribe secuencia de inicializacion
	I2C_wait(I2C_0); //Checking if the I2C module is busy by cheking the busy flag
	I2C_get_ack(I2C_0); //Waiting for acknowledge

	I2C_write_byte(I2C_0, 0x02); //Escribe secuencia de inicializacion
	I2C_wait(I2C_0); //Checking if the I2C module is busy by cheking the busy flag
	I2C_get_ack(I2C_0); //Waiting for acknowledge

	I2C_write_byte(I2C_0, 0x10); //Escribe secuencia de inicializacion
	I2C_wait(I2C_0); //Checking if the I2C module is busy by cheking the busy flag
	I2C_get_ack(I2C_0); //Waiting for acknowledge

	I2C_write_byte(I2C_0, 0x01); //Escribe secuencia de inicializacion
	I2C_wait(I2C_0); //Checking if the I2C module is busy by cheking the busy flag
	I2C_get_ack(I2C_0); //Waiting for acknowledge

	I2C_stop(I2C_0);

}

void Matricial_LEDs_write(uint64_t character1, uint64_t character2) {
	delay(DELAY);

	I2C_start(I2C_0); //it configures the I2C in trasnmitting mode and generates the start

	I2C_write_byte(I2C_0, HT16K33_WRITE_ADDRESS); //Escribe la direccion de la matriz de LEDs
	I2C_wait(I2C_0); //Checking if the I2C module is busy by cheking the busy flag
	I2C_get_ack(I2C_0); //Waiting for acknowledge

	I2C_write_byte(I2C_0, 0x00); //Mem Address
	I2C_wait(I2C_0); //Checking if the I2C module is busy by cheking the busy flag
	I2C_get_ack(I2C_0); //Waiting for acknowledge

	uint8_t x1[OCHO_PINOCHO];
	uint8_t x2[OCHO_PINOCHO];
	uint8_t i;

	for (i = 0; i < OCHO_PINOCHO; i++) {
		x1[i] = character1 >> (OCHO_PINOCHO * (7 - i)); //Almacena la columna del numero al recorrer todos los numeros 8 * (7-i) solo dejar el
		x2[i] = character2 >> (OCHO_PINOCHO * (7 - i)); // byte que no interesa imprimir
	}

	for (i = 0; i < OCHO_PINOCHO; i++) {
		I2C_write_byte(I2C_0, x1[i]); //Mem Address
		I2C_wait(I2C_0); //Checking if the I2C module is busy by cheking the busy flag
		I2C_get_ack(I2C_0); //Waiting for acknowledge

		I2C_write_byte(I2C_0, x2[i]); //Mem Address
		I2C_wait(I2C_0); //Checking if the I2C module is busy by cheking the busy flag
		I2C_get_ack(I2C_0); //Waiting for acknowledge
	}

	I2C_stop(I2C_0);
}

void Matricial_LEDs_clear(void) {
	delay(DELAY);

	I2C_start(I2C_0); //it configures the I2C in trasnmitting mode and generates the start

	I2C_write_byte(I2C_0, HT16K33_WRITE_ADDRESS); //Escribe la direccion de la matriz de LEDs
	I2C_wait(I2C_0); //Checking if the I2C module is busy by cheking the busy flag
	I2C_get_ack(I2C_0); //Waiting for acknowledge

	I2C_write_byte(I2C_0, 0x00); //Mem Address
	I2C_wait(I2C_0); //Checking if the I2C module is busy by cheking the busy flag
	I2C_get_ack(I2C_0); //Waiting for acknowledge

	uint8_t i;
	for (i = 0; i < OCHO_PINOCHO; i++) {
		I2C_write_byte(I2C_0, 0x00); //Mem Address
		I2C_wait(I2C_0); //Checking if the I2C module is busy by cheking the busy flag
		I2C_get_ack(I2C_0); //Waiting for acknowledge

		I2C_write_byte(I2C_0, 0x00); //Mem Address
		I2C_wait(I2C_0); //Checking if the I2C module is busy by cheking the busy flag
		I2C_get_ack(I2C_0); //Waiting for acknowledge
	}

	I2C_stop(I2C_0);

}

void Matricial_LEDs_put_character(uint64_t character1, uint64_t character2,
		uint64_t next_char) {
	g_character1 = character1;
	g_character2 = character2;
	g_next_char = next_char;
}

void Matricial_LEDs_shift_character(void) {
	if (!I2C_busy(I2C_0)) {
		Matricial_LEDs_write(g_character1, g_character2);

		uint8_t temp_next = 0;
		uint8_t temp2 = 0;

		temp_next = g_next_char >> 56; // Guardamos el byte mas significativo de la cadena que sigue
		g_next_char = g_next_char << 8; //Recorremos la cadena que sigue
		temp2 = g_character2 >> 56; // Guardamos el byte mas significativo de la segunda matriz de LEDs
		g_character1 = (g_character1 << 8) + temp2; //Recorremos un registro el caracter de la primera pantala y le vamos añadiendo el segundo
		g_character2 = (g_character2 << 8) + temp_next; //introducimos byte a byte el nuevo caracter
		PIT_clear2_interrupt_flag();
		g_cont++;
		if (g_cont == 8) {
			Matricial_LEDs_change_character();
			g_cont = 0;
		}

	}
}

void Matricial_LEDs_clear_gString(void)
{
	uint8_t i;
	for(i = 0; i < 30; i++)
	{
		g_string[i] = '\0';
	}
}

void Matricial_LEDs_set_string(uint8_t* cadena) {
	uint8_t i = 0;
	while (*cadena != '\0') {
		g_string[i] = Matrcicial_LEDs_ascii_decoder(*cadena); //Almacena los valores decodificados en un arreglo
		*cadena++; //recorre la posicion de la cadena
		i++;
	}
	Matricial_LEDs_put_character(g_string[0], g_string[1], g_string[2]);

}

void Matricial_LEDs_change_character(void) {
	g_character1 = g_string[1]; //almacenamos la siguiente posicion como iniclal
	g_character2 = g_string[2]; //el nuevo caracter en la segunda matriz
	g_next_char = g_string[3]; // el 3ro sera el siguiente caracter
	uint64_t aux = g_string[0]; // almacenamos el primer valor para ponerlo al final del arreglo
	uint8_t i = 0;

	while (g_string[i + 1] != '\0') {
		g_string[i] = g_string[i + 1]; //recorre una posicion el arreglo
		i++;
	}
	g_string[i] = aux; //a la ultima posicion se le pone la primera, para que siga repitiendo la cadena

}

void Matricial_LEDs_modo_hora(void) {
	if (!I2C_busy(I2C_0)) {
		if (CONECTADO == estadoRTCC) {
			RTCC_ReadDateTimeFull();
			if (FALSE == RTCC_getGlobalVariable(AMPM)) {
				if (12 == RTCC_getGlobalVariable(HOURS)) {
					g_character1 = Matrcicial_LEDs_decimal_decoder(0); //obtenemos los valores en decimal y los decodificamos
				} else {
					g_character1 = Matrcicial_LEDs_decimal_decoder(
							RTCC_getGlobalVariable(HOURS)); //obtenemos los valores en decimal y los decodificamos
				}
			} else {
				if (12 == RTCC_getGlobalVariable(HOURS)) {
					g_character1 = Matrcicial_LEDs_decimal_decoder(
							RTCC_getGlobalVariable(HOURS)); //obtenemos los valores en decimal y los decodificamos
				} else {
					g_character1 = Matrcicial_LEDs_decimal_decoder(
							RTCC_getGlobalVariable(HOURS) + 12); //obtenemos los valores en decimal y los decodificamos
				}
			}
			g_character2 = Matrcicial_LEDs_decimal_decoder(
					RTCC_getGlobalVariable(MINUTES));

			g_character1 = g_character1 | 0x0200000000000000; //toggle seconds

			Matricial_LEDs_write(g_character1, g_character2); //imprimimos la hora
			PIT_clear1_interrupt_flag(); //limpiamos la bandera
		} else {
			g_character1 = Matrcicial_LEDs_ascii_decoder('x'); //obtenemos los valores en decimal y los decodificamos
			g_character2 = Matrcicial_LEDs_ascii_decoder('x');
		}
	}
}

void Matricial_LEDs_toggle_seg(void) {
	if (!I2C_busy(I2C_0)) {
		RTCC_ReadDateTimeFull();
		g_character1 = g_character1 - 0x0100000000000000; //toggle seconds
		Matricial_LEDs_write(g_character1, g_character2); //imprimimos la hora
		PIT_clear0_interrupt_flag(); //limpiamos la bandera
	}
}

void Matricial_LEDs_Modo_Hora_init(void) {
	NVIC_enable_interrupt_and_priotity(PIT_CH1_IRQ, PRIORITY_5);
	NVIC_enable_interrupt_and_priotity(PIT_CH0_IRQ, PRIORITY_6);
	NVIC_global_enable_interrupts;

	PIT_clock_gating(); //inicializamos el pit
	PIT_enable();

	PIT_delay(PIT_1, SYSTEM_CLOCK_RATE, SEG_DELAY); //Inicializaciones del PIT
	PIT_enable_interrupt_1(PIT_1);

	PIT_delay(PIT_0, SYSTEM_CLOCK_RATE, HALF_SEG_DELAY); //Inicializaciones del PIT
	PIT_enable_interrupt_0(PIT_0);

	PIT_callback_init(PIT_1, &Matricial_LEDs_modo_hora);
	PIT_callback_init(PIT_0, &Matricial_LEDs_toggle_seg);

}

void Matricial_LEDs_Modo_cadena_init(void) {
	NVIC_enable_interrupt_and_priotity(PIT_CH2_IRQ, PRIORITY_6);
	NVIC_global_enable_interrupts;

	PIT_clock_gating(); //inicializaciones del PIT
	PIT_enable();
	PIT_delay(PIT_2, SYSTEM_CLOCK_RATE, PIT_DELAY);
	PIT_enable_interrupt_2(PIT_2);

	PIT_callback_init(PIT_2, &Matricial_LEDs_shift_character);

}

void Matricial_LEDs_Modo_hora_callback_off(void) {
	PIT_callback_init(PIT_1, 0);
	PIT_callback_init(PIT_0, 0);
}

void Matricial_LEDs_Modo_cadena_callback_off(void) {
	PIT_callback_init(PIT_2, 0);
}

uint64_t Matrcicial_LEDs_decimal_decoder(uint8_t cadena) { //Conversion de Decimal al valor hexadecimal del número a 64bits (8 bytes)
	switch (cadena) {
	case 0:
		return (0x00FF81FF00FF81FF); //0
		break;
	case 1:
		return (0x00FF81FF00217F01); //1
		break;
	case 2:
		return (0x00FF81FF008F89F9);
		break;
	case 3:
		return (0x00FF81FF008989FF);
		break;
	case 4:
		return (0x00FF81FF00F010FF);
		break;
	case 5:
		return (0x00FF81FF00F9898F);
		break;
	case 6:
		return (0x00FF81FF00FF898F);
		break;
	case 7:
		return (0x00FF81FF008080FF);
		break;
	case 8:
		return (0x00FF81FF00FF89FF);
		break;
	case 9:
		return (0x00FF81FF00F888FF);
		break;

		/////////10-19

	case 10:
		return (0x00217F0100FF81FF); //0
		break;
	case 11:
		return (0x00217F0100217F01); //1
		break;
	case 12:
		return (0x00217F01008F89F9);
		break;
	case 13:
		return (0x00217F01008989FF);
		break;
	case 14:
		return (0x00217F0100F010FF);
		break;
	case 15:
		return (0x00217F0100F9898F);
		break;
	case 16:
		return (0x00217F0100FF898F);
		break;
	case 17:
		return (0x00217F01008080FF);
		break;
	case 18:
		return (0x00217F0100FF89FF);
		break;
	case 19:
		return (0x00217F0100F888FF);
		break;

		//20-29

	case 20:
		return (0x008F89F900FF81FF); //0
		break;
	case 21:
		return (0x008F89F900217F01); //1
		break;
	case 22:
		return (0x008F89F9008F89F9);
		break;
	case 23:
		return (0x008F89F9008989FF);
		break;
	case 24:
		return (0x008F89F900F010FF);
		break;
	case 25:
		return (0x008F89F900F9898F);
		break;
	case 26:
		return (0x008F89F900FF898F);
		break;
	case 27:
		return (0x008F89F9008080FF);
		break;
	case 28:
		return (0x008F89F900FF89FF);
		break;
	case 29:
		return (0x008F89F900F888FF);
		break;

		//30-39

	case 30:
		return (0x008989FF00FF81FF); //0
		break;
	case 31:
		return (0x008989FF00217F01); //1
		break;
	case 32:
		return (0x008989FF008F89F9);
		break;
	case 33:
		return (0x008989FF008989FF);
		break;
	case 34:
		return (0x008989FF00F010FF);
		break;
	case 35:
		return (0x008989FF00F9898F);
		break;
	case 36:
		return (0x008989FF00FF898F);
		break;
	case 37:
		return (0x008989FF008080FF);
		break;
	case 38:
		return (0x008989FF00FF89FF);
		break;
	case 39:
		return (0x008989FF00F888FF);
		break;

		//40-49

	case 40:
		return (0x00F010FF00FF81FF); //0
		break;
	case 41:
		return (0x00F010FF00217F01); //1
		break;
	case 42:
		return (0x00F010FF008F89F9);
		break;
	case 43:
		return (0x00F010FF008989FF);
		break;
	case 44:
		return (0x00F010FF00F010FF);
		break;
	case 45:
		return (0x00F010FF00F9898F);
		break;
	case 46:
		return (0x00F010FF00FF898F);
		break;
	case 47:
		return (0x00F010FF008080FF);
		break;
	case 48:
		return (0x00F010FF00FF89FF);
		break;
	case 49:
		return (0x00F010FF00F888FF);
		break;

		//50-59

	case 50:
		return (0x00F9898F00FF81FF); //0
		break;
	case 51:
		return (0x00F9898F00217F01); //1
		break;
	case 52:
		return (0x00F9898F008F89F9);
		break;
	case 53:
		return (0x00F9898F008989FF);
		break;
	case 54:
		return (0x00F9898F00F010FF);
		break;
	case 55:
		return (0x00F9898F00F9898F);
		break;
	case 56:
		return (0x00F9898F00FF898F);
		break;
	case 57:
		return (0x00F9898F008080FF);
		break;
	case 58:
		return (0x00F9898F00FF89FF);
		break;
	case 59:
		return (0x00F9898F00F888FF);
		break;
	}
}

uint64_t Matrcicial_LEDs_ascii_decoder(uint8_t cadena) { //Conversion de Ascii al valor hexadecimal del numero a 64bits (8 bytes)
	switch (cadena) {
	case '0':
		return (0xFF81FF); //0
		break;
	case '1':
		return (0x217F01); //1
		break;
	case '2':
		return (0x8F89F9);
		break;
	case '3':
		return (0x8989FF);
		break;
	case '4':
		return (0xF010FF);
		break;
	case '5':
		return (0xF9898F);
		break;
	case '6':
		return (0xFF898F);
		break;
	case '7':
		return (0x8080FF);
		break;
	case '8':
		return (0xFF89FF);
		break;
	case '9':
		return (0xF888FF);
		break;

	case 'a':
		return (0x003F7FC8C87F3F00);
		break;
	case 'A':
		return (0x003F7FC8C87F3F00);
		break;

	case 'b':
		return (0x007F7F49493F3600);
		break;
	case 'B':
		return (0x007F7F49493F3600);
		break;

	case 'c':
		return (0x003E7F4141632200);
		break;
	case 'C':
		return (0x003E7F4141632200);
		break;

	case 'd':
		return (0x007F7F41417F3E00);
		break;
	case 'D':
		return (0x007F7F41417F3E00);
		break;

	case 'e':
		return (0x007F7F4949494100);
		break;
	case 'E':
		return (0x007F7F4949494100);
		break;

	case 'f':
		return (0x007F7F4848484000);
		break;
	case 'F':
		return (0x007F7F4848484000);
		break;

	case 'g':
		return (0x003E7F4145672600);
		break;
	case 'G':
		return (0x003E7F4145672600);
		break;

	case 'h':
		return (0x007F7F08087F7F00);
		break;
	case 'H':
		return (0x007F7F08087F7F00);
		break;

	case 'i':
		return (0x0000417F7F410000);
		break;
	case 'I':
		return (0x0000417F7F410000);
		break;

	case 'j':
		return (0x000607417F7E4000);
		break;
	case 'J':
		return (0x000607417F7E4000);
		break;

	case 'k':
		return (0x007F7F1C36634100);
		break;
	case 'K':
		return (0x007F7F1C36634100);
		break;

	case 'l':
		return (0x007F7F0101010100);
		break;
	case 'L':
		return (0x007F7F0101010100);
		break;

	case 'm':
		return (0x007F7F3018307F7F);
		break;
	case 'M':
		return (0x007F7F3018307F7F);
		break;

	case 'n':
		return (0x007F7F30180C7F7F);
		break;
	case 'N':
		return (0x007F7F30180C7F7F);
		break;

	case 'o':
		return (0x003E7F41417F3E00);
		break;
	case 'O':
		return (0x003E7F41417F3E00);
		break;

	case 'p':
		return (0x007F7F44447C3800);
		break;
	case 'P':
		return (0x007F7F44447C3800);
		break;

	case 'q':
		return (0x003C7E42427F3D00);
		break;
	case 'Q':
		return (0x003C7E42427F3D00);
		break;

	case 'r':
		return (0x007F7F4C4E7B3100);
		break;
	case 'R':
		return (0x007F7F4C4E7B3100);
		break;

	case 's':
		return (0x00327B49496F2600);
		break;
	case 'S':
		return (0x00327B49496F2600);
		break;

	case 't':
		return (0x0060607F7F606000);
		break;
	case 'T':
		return (0x0060607F7F606000);
		break;

	case 'u':
		return (0x007E7F01017F7F00);
		break;
	case 'U':
		return (0x007E7F01017F7F00);
		break;

	case 'v':
		return (0x00FCFE0303FEFC00);
		break;
	case 'V':
		return (0x00FCFE0303FEFC00);
		break;

	case 'w':
		return (0x007F7F060C067F7F);
		break;
	case 'W':
		return (0x007F7F060C067F7F);
		break;

	case 'x':
		return (0x0060771C081C7763);
		break;
	case 'X':
		return (0x0060771C081C7763);
		break;

	case 'y':
		return (0x0070780F0F787000);
		break;
	case 'Y':
		return (0x0070780F0F787000);
		break;

	case 'z':
		return (0x0043474D59716100);
		break;
	case 'Z':
		return (0x0043474D59716100);
		break;

	case ' ':
		return (0x8000000000000000);
		break;
	case '  ':
		return (0x8000000000000000);
		break;

	default:
		return 0;
		break;
	}
}

