/*
 * RAM.c
 *
 *  Created on: 21/04/2019
 *      Author: best buy
 */

#include "RAM.h"
#include "Delay.h"

#define DELAY_RAM 10000

uint8_t  g_read_data[30];

void RAM_write(UART_StringNumber string , uint8_t* data)
{


	delay(DELAY_RAM);

	uint8_t low_add;

	switch(string)
	{
	case STRING_UNO:
		low_add = RAM_C1_LOW_ADDRESS;
		break;
	case STRING_DOS:
		low_add = RAM_C2_LOW_ADDRESS;
		break;
	case STRING_TRES:
		low_add = RAM_C3_LOW_ADDRESS;
		break;
	case STRING_CUATRO:
		low_add = RAM_C4_LOW_ADDRESS;
		break;
	case STRING_CINCO:
		low_add = RAM_C5_LOW_ADDRESS;
		break;
	}

	I2C_start(I2C_0); //it configures the I2C in trasnmitting mode and generates the start

	I2C_write_byte(I2C_0, RAM_WRITE_ADRESS); //Escribe la direccion de la RAM
	I2C_wait(I2C_0); //Checking if the I2C module is busy by cheking the busy flag
	I2C_get_ack(I2C_0); //Waiting for acknowledge

	I2C_write_byte(I2C_0, RAM_HIGH_ADDRESS);
	I2C_wait(I2C_0); //Checking if the I2C module is busy by cheking the busy flag
	I2C_get_ack(I2C_0); //Waiting for acknowledge

	I2C_write_byte(I2C_0, low_add); //Escribe secuencia de inicializacion
	I2C_wait(I2C_0); //Checking if the I2C module is busy by cheking the busy flag
	I2C_get_ack(I2C_0); //Waiting for acknowledge

 while(*data != '\0')
 {
	I2C_write_byte(I2C_0, *data); //Escribe secuencia de inicializacion
	I2C_wait(I2C_0); //Checking if the I2C module is busy by cheking the busy flag
	I2C_get_ack(I2C_0); //Waiting for acknowledge
	*data++;
 }


	I2C_stop(I2C_0);
	uint8_t i;
	for(i = 0; i < 30; i++)
	{
		g_read_data[i] = '\0';
	}


}

uint8_t RAM_read(uint8_t low_add)
{

	delay(DELAY_RAM);

	uint8_t cadena; //Variable donde se almacenará la lectura

	I2C_start(I2C_0); //it configures the I2C in trasnmitting mode and generates the start

	I2C_write_byte(I2C_0, RAM_WRITE_ADRESS); //Escribe la direccion de la RAM
	I2C_wait(I2C_0); //Checking if the I2C module is busy by cheking the busy flag
	I2C_get_ack(I2C_0); //Waiting for acknowledge

	I2C_write_byte(I2C_0, RAM_HIGH_ADDRESS);
	I2C_wait(I2C_0); //Checking if the I2C module is busy by cheking the busy flag
	I2C_get_ack(I2C_0); //Waiting for acknowledge

	I2C_write_byte(I2C_0, low_add); //Escribe secuencia de inicializacion
	I2C_wait(I2C_0); //Checking if the I2C module is busy by cheking the busy flag
	I2C_get_ack(I2C_0); //Waiting for acknowledge

	I2C_repeted_start(I2C_0); //Generating a new start for Reading

	I2C_write_byte(I2C_0, RAM_READ_ADRESS); //Escribe la direccion de la RAM
	I2C_wait(I2C_0); //Checking if the I2C module is busy by cheking the busy flag
	I2C_get_ack(I2C_0); //Waiting for acknowledge

	I2C_tx_rx_mode(I2C_0, RECEIVER_MODE); //Changing i2c module to receiver mode

	cadena = I2C_read_byte(I2C_0); //Dummy Read

	I2C_wait(I2C_0); //Checking if the I2C module is busy by cheking the busy flag

	I2C_nack(I2C_0); //generating not acknowledge

	I2C_stop(I2C_0);

	cadena = I2C_read_byte(I2C_0); //clean read



	return cadena;

}

uint8_t* RAM_read_string(UART_StringNumber string)
{

	uint8_t low_add;
	switch(string)
	{
	case STRING_UNO:
		low_add = RAM_C1_LOW_ADDRESS;
		break;
	case STRING_DOS:
		low_add = RAM_C2_LOW_ADDRESS;
		break;
	case STRING_TRES:
		low_add = RAM_C3_LOW_ADDRESS;
		break;
	case STRING_CUATRO:
		low_add = RAM_C4_LOW_ADDRESS;
		break;
	case STRING_CINCO:
		low_add = RAM_C5_LOW_ADDRESS;
		break;
	}
	RAM_read(low_add);
	uint8_t i = 0;
 for(i = 0; i < 30; i++)
 {
	 g_read_data[i] = RAM_read(low_add + i);
	 if(g_read_data[i] == 255)
	 {
		 break; //si encuentra  basura significa que la cadena terminó y trunca el ciclo for
	 }
 }

 uint8_t* cadena;
 cadena = &g_read_data[0];
 return cadena;


}
