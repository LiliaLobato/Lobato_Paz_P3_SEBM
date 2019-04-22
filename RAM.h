/*
 * RAM.h
 *
 *  Created on: 21/04/2019
 *      Author: best buy
 */

#ifndef RAM_H_
#define RAM_H_

#include "I2C.h"
#include "UART_string.h"

#define RAM_WRITE_ADRESS 0xA0
#define RAM_READ_ADRESS 0xA1
#define RAM_HIGH_ADDRESS 0x00
#define RAM_C1_LOW_ADDRESS 0x00
#define RAM_C2_LOW_ADDRESS 0x40 //NEXT PAGE (+64BYTES)
#define RAM_C3_LOW_ADDRESS 0x80//NEXT PAGE (+64BYTES)
#define RAM_C4_LOW_ADDRESS 0xC0 //NEXT PAGE (+64BYTES)
#define RAM_C5_LOW_ADDRESS 0xDF //+30 (NUMERO DE CARACTERES MAXIMO)

void RAM_write(UART_StringNumber string , uint8_t* data);

uint8_t RAM_read(uint8_t low_add);

uint8_t* RAM_read_string(UART_StringNumber string);


#endif /* RAM_H_ */
