/*
 * Matrcial_LEDs.h
 *
 *  Created on: 19/04/2019
 *      Author: best buy
 */

#ifndef MATRCIAL_LEDS_H_
#define MATRCIAL_LEDS_H_

#include "I2C.h"
#include "stdint.h"
#include "MK64F12.h"

#define LETTER_A "A"
#define LETTER_B "B"
#define OCHO_PINOCHO 8

typedef struct
{
	uint32_t character;
	uint64_t hex_value;
}ABC_t;

typedef uint8_t* uint8_pointer_t;



void Matricial_LEDs_init(void);

void Matricial_LEDs_test(void);

void Matricial_LEDs_write(uint64_t character1, uint64_t character2);

void Matricial_LEDs_clear(void);

void Matricial_LEDs_shift_character(void);

void Matricial_LEDs_put_character(uint64_t character1, uint64_t character2, uint64_t next_char);

uint64_t Matrcicial_LEDs_ascii_decoder(uint8_t cadena);

void Matricial_LEDs_loop_string(uint8_t* cadena);

void Matricial_LEDs_modo_hora(void);



/*ABC_t ABC[] =
{
		{"A", 0x003F7FC8C87F3F00},
		{"B", 0x007F7F49493F3600},
		{"B", 0x007F7F49493F3600},
};

ABC_t numbers[10] =
{
		{1, 0x017F21},
		{2, 0xF9898F},
		{3, 0xFF8989},
		{4, 0xFF10F0},
		{5, 0x8F89F9},
		{6, 0x8F89FF},
		{7, 0xFF8080},
		{8, 0xFF89FF},
		{9, 0xFF88F8},
		{0, 0xFF81FF},
};*/


#endif /* MATRCIAL_LEDS_H_ */
