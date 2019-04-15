/*
 * I2C.c
 *
 *  Created on: 09/04/2019
 *      Author: best buy
 */
#include "I2C.h"



void I2C_init(const I2C_config_struct_t* g_I2C_config)
{
	//GPIO_pin_control_register(g_I2C_config->port_name, g_I2C_config->SDA_pin, g_I2C_config->I2C_Pin_config); //Pin as SDA
	//GPIO_pin_control_register(g_I2C_config->port_name, g_I2C_config->SCL_pin, g_I2C_config->I2C_Pin_config); //Pin as SCL

	PORTB->PCR[3] |= PORT_PCR_MUX(0x2) | GPIO_PFE | GPIO_PE | GPIO_PS;
	PORTB->PCR[2] |= PORT_PCR_MUX(0x2) | GPIO_PFE | GPIO_PE | GPIO_PS;

	SIM->SCGC4 |= g_I2C_config->clock_gating; //Enables the I2C0 Clock Gating
	I2C0->C1 |= g_I2C_config->I2C_Enable;
	I2C0->F |= I2Cn_F_MULT2_MASK;
	I2C0->F |= g_I2C_config->ICR_value; //SCL div of 26 (400KHz)
	I2C_mst_or_slv_mode(MASTER);
	I2C_tx_rx_mode(TRANSMITTER_MODE);
	I2C0->SMB |= I2Cn_SMB_FACK_MASK; //NACK/ACK on this Byte instead of the following one
}


void I2C_mst_or_slv_mode(master_or_slave_t mst_or_slv)
{
	I2C0->C1 &= ~I2Cn_C1_MST_MASK; //Set as slave
	if(MASTER == mst_or_slv)
	{
		I2C0->C1 |= I2Cn_C1_MST_MASK; //Set as master
	}
	else
	{
		I2C0->C1 &= ~I2Cn_C1_MST_MASK; //Set as slave
	}
}

void I2C_nack(void)
{
	I2C0->C1 |= I2C_C1_TXAK(TRUE);//I2Cn_C1_TXAK_MASK; //No Acknowledge signal response is sent

}

void I2C_write_byte(uint8_t data)
{
	I2C0->D = data; //Data transfer starts when data is written
}

void I2C_wait(void)
{
	while((FALSE == (I2C0->S & I2C_S_IICIF_MASK) >> I2C_S_IICIF_SHIFT) && FALSE == (I2C0->S & I2C_S_TCF_MASK))
	{

	}
	I2C0->S |= I2C_S_IICIF_MASK; //Write 1 to clear this bit (w1c)
	//I2C0->S |= I2C_S_SRW_MASK; // .h says that this bit must be set. However, this bit is for read only
}

void I2C_start(void)
{
	I2C0->C1 |= I2C_C1_MST_MASK; // When MST bit of C1 register changes from 0 to 1, a START signal is generated
	I2C_tx_rx_mode(TRANSMITTER_MODE); //Changes to transmitter mode
}

/////////////////////////////////////////////////////////////////////////////////////////////////

uint8_t I2C_busy(void) {
	return ((I2C0->S & I2C_S_BUSY_MASK) >> I2C_S_BUSY_SHIFT); // 1 BUSY, 0 idle
}

void I2C_tx_rx_mode(tx_or_rx_mode_t tx_or_rx) {
	if(TRANSMITTER_MODE == tx_or_rx){
		I2C0->C1 |= I2C_C1_TX_MASK; //Set as trasmitter 1
	}else{
		I2C0->C1 &= ~(I2C_C1_TX_MASK); //Set as Receiver 0
	}
}

void I2C_repeted_start(void){
	I2C0->C1 |= I2C_C1_RSTA_MASK; //writes 1 in the RSTA bit generating a  repeated START condition
	//I2C_C1_MST(0b1); //Preguntar sobre usar esto
}

/*!
 	 \brief
 	 	 It reads data from the receiving buffer.
 	 \return void
 */
uint8_t  I2C_read_byte(void){
 return I2C0->D; //Retorna el valor del buffer
}

uint8_t I2C_get_ack(void){
	return(I2C0->S & I2C_S_RXAK_MASK); //0 received, 1 not received
}


void I2C_stop(void){
	I2C0->C1 &= ~I2C_C1_TXAK_MASK; /* An acknowledge signal is sent to the bus on the following receiving byte */
	I2C_mst_or_slv_mode(SLAVE); // When MST bit of C1 register changes from 1 to 0, a STOP signal is generated
	I2C_tx_rx_mode(RECEIVER_MODE); //Changes to receiver mode
}
