/*
 * MCP7940M.c
 *
 *  Created on: 10/04/2019
 *      Author: Lilia Lobato
 */
#include "MCP7940M.h"

void MCP7940M_tSeconds(){
	/*uint8_t dataFromMCP7940M;
	I2C_start(); //it configures the I2C in trasnmitting mode and generates the start

	I2C_write_byte(0xDE); //Writing RTC address as write //11011110
	I2C_wait(); //Checking if the I2C module is busy by cheking the busy flag
	I2C_get_ack(); //Waiting for acknowledge

	I2C_write_byte(0x00); //Writing RTC address in the data register
	I2C_wait(); //Checking if the I2C module is busy by cheking the busy flag
	I2C_get_ack(); //Waiting for acknowledge

	I2C_repeted_start(); //Generating a new start
	I2C_write_byte(0xDF); //Writing slave direction as read //11011111
	I2C_wait();
	I2C_get_ack();

	I2C_tx_rx_mode(RECEIVER_MODE); //Changing i2c module to receiver mode

	I2C_nack(); //generating not acknowledge
	dataFromMCP7940M = I2C_read_byte();
	I2C_wait();

	I2C_stop();
	dataFromMCP7940M = I2C_read_byte();
*/
	//return (dataFromMCP7940M);

}
