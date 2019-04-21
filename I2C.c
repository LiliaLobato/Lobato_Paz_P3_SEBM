/*
 * I2C.c
 *
 *  Created on: 09/04/2019
 *      Author: best buy
 */
#include "I2C.h"

void I2C_init(const I2C_config_struct_t* g_I2C_config) {
	//GPIO_pin_control_register(g_I2C_config->port_name, g_I2C_config->SDA_pin, g_I2C_config->I2C_Pin_config); //Pin as SDA
	//GPIO_pin_control_register(g_I2C_config->port_name, g_I2C_config->SCL_pin, g_I2C_config->I2C_Pin_config); //Pin as SCL
	gpio_pin_control_register_t MUX_alternative_0 = GPIO_MUX2 | GPIO_PFE
			| GPIO_PE | GPIO_PS;
	gpio_pin_control_register_t MUX_alternative_1 = GPIO_MUX2 | GPIO_PFE
			| GPIO_PE | GPIO_PS;
	gpio_pin_control_register_t MUX_alternative_2 = GPIO_MUX5 | GPIO_PFE
			| GPIO_PE | GPIO_PS;

	switch (g_I2C_config->channel) {
	case I2C_0:
		/** Enable the clock gating for I2C0 */
		SIM->SCGC4 |= g_I2C_config->clock_gating; //Enables the I2C0 Clock Gating
		/** Configure the pins for SDA and SCL */
		GPIO_pin_control_register(GPIO_B, bit_2, &MUX_alternative_0);
		GPIO_pin_control_register(GPIO_B, bit_3, &MUX_alternative_0);
		/** Assign the Mult value and the ICR value*/
		I2C0->F |= (I2C_F_MULT(bit_1) | g_I2C_config->ICR_value);
		// An Ack/Nack can be sent
		I2C0->SMB |= I2C_SMB_FACK_MASK;
		/** Enable the I2C0 module*/
		I2C0->C1 |= g_I2C_config->I2C_Enable;
		I2C0->C1 |= I2C_C1_IICEN_MASK;
		break;
	case I2C_1:
		/** Enable the clock gating for I2C0 */
		SIM->SCGC4 |= g_I2C_config->clock_gating;
		/** Configure the pins for SDA and SCL */
		GPIO_pin_control_register(GPIO_C, bit_10, &MUX_alternative_1);
		GPIO_pin_control_register(GPIO_C, bit_11, &MUX_alternative_1);
		/** Assign the Mult value and the ICR value*/
		I2C1->F = (I2C_F_MULT(bit_2) | g_I2C_config->ICR_value);
		// An Ack/Nack can be sent
		I2C1->SMB |= I2C_SMB_FACK_MASK;
		/** Enable the I2C0 module*/
		I2C0->C1 |= g_I2C_config->I2C_Enable;
		I2C1->C1 |= I2C_C1_IICEN_MASK;
		break;
	case I2C_2:
		SIM->SCGC1 |= g_I2C_config->clock_gating;
		/** Configure the pins for SDA and SCL */
		GPIO_pin_control_register(GPIO_A, bit_11, &MUX_alternative_2);
		GPIO_pin_control_register(GPIO_A, bit_12, &MUX_alternative_2);
		/** Assign the Mult value and the ICR value*/
		I2C2->F = (I2C_F_MULT(bit_2) | g_I2C_config->ICR_value);
		// An Ack/Nack can be sent
		I2C2->SMB |= I2C_SMB_FACK_MASK;
		/** Enable the I2C0 module*/
		I2C0->C1 |= g_I2C_config->I2C_Enable;
		I2C2->C1 |= I2C_C1_IICEN_MASK;
		break;
	default:
		break;
	}

	I2C_mst_or_slv_mode(g_I2C_config->channel, MASTER);
	I2C_tx_rx_mode(g_I2C_config->channel, TRANSMITTER_MODE);
}

void I2C_mst_or_slv_mode(i2c_channel_t channel, master_or_slave_t mst_or_slv) {
	switch (channel) {
	case I2C_0:
		if (MASTER == mst_or_slv) {
			I2C0->C1 |= I2C_C1_MST_MASK; //Set as master
		} else {
			I2C0->C1 &= ~I2C_C1_MST_MASK; //Set as slave
		}

		break;
	case I2C_1:
		if (MASTER == mst_or_slv) {
			I2C1->C1 |= I2C_C1_MST_MASK; //Set as master
		} else {
			I2C1->C1 &= ~I2C_C1_MST_MASK; //Set as slave
		}

		break;
	case I2C_2:
		if (MASTER == mst_or_slv) {
			I2C2->C1 |= I2C_C1_MST_MASK; //Set as master
		} else {
			I2C2->C1 &= ~I2C_C1_MST_MASK; //Set as slave
		}

		break;
	default:
		break;
	}

}

void I2C_nack(i2c_channel_t channel) {
	switch (channel) {
	case I2C_0:
		I2C0->C1 |= I2C_C1_TXAK_MASK;
		break;
	case I2C_1:
		I2C1->C1 |= I2C_C1_TXAK_MASK;
		break;
	case I2C_2:
		I2C2->C1 |= I2C_C1_TXAK_MASK;
		break;
	default:
		break;
	}
	//I2Cn_C1_TXAK_MASK; //No Acknowledge signal response is sent

}

void I2C_write_byte(i2c_channel_t channel, uint8_t data) {
	switch (channel) {
	case I2C_0:
		I2C0->D = data;
		break;
	case I2C_1:
		I2C1->D = data;
		break;
	case I2C_2:
		I2C2->D = data;
		break;
	default:
		break;
	} //Data transfer starts when data is written
}

void I2C_wait(i2c_channel_t channel) {

	switch (channel) {
	uint8_t notStay=0;
	case I2C_0:
		while (FALSE == ((I2C0->S & I2C_S_IICIF_MASK) >> I2C_S_IICIF_SHIFT)){
		}
		I2C0->S |= I2C_S_IICIF_MASK;
		//Write 1 to clear this bit (w1c)
		break;
	case I2C_1:
		while (FALSE == (I2C1->S & I2C_S_IICIF_MASK))
			;
		I2C1->S |= I2C_S_IICIF_MASK;
		break;
	case I2C_2:
		while (FALSE == (I2C2->S & I2C_S_IICIF_MASK))
			;
		I2C2->S |= I2C_S_IICIF_MASK;
		break;
	default:
		notStay++;
		break;
	}

	//I2C0->S |= I2C_S_SRW_MASK; // .h says that this bit must be set. However, this bit is for read only
}

void I2C_start(i2c_channel_t channel) {
	switch (channel) {
	case I2C_0:
		I2C0->C1 |= (I2C_C1_MST_MASK | I2C_C1_TX_MASK); // When MST bit of C1 register changes from 0 to 1, a START signal is generated
		//Changes to transmitter mode
		break;
	case I2C_1:
		I2C1->C1 |= (I2C_C1_MST_MASK | I2C_C1_TX_MASK);
		break;
	case I2C_2:
		I2C2->C1 |= (I2C_C1_MST_MASK | I2C_C1_TX_MASK);
		break;
	default:
		break;
	}
}

/////////////////////////////////////////////////////////////////////////////////////////////////

uint8_t I2C_busy(i2c_channel_t channel) {
	uint8_t busy_status;

	switch (channel) {
	case I2C_0:
		busy_status = (I2C0->S & I2C_S_BUSY_MASK) >> I2C_S_BUSY_SHIFT;
		break;
	case I2C_1:
		busy_status = (I2C1->S & I2C_S_BUSY_MASK) >> I2C_S_BUSY_SHIFT;
		break;
	case I2C_2:
		busy_status = (I2C2->S & I2C_S_BUSY_MASK) >> I2C_S_BUSY_SHIFT;
		break;
	default:
		break;
	}

	return (busy_status); // 1 BUSY, 0 idle
}

void I2C_tx_rx_mode(i2c_channel_t channel, tx_or_rx_mode_t tx_or_rx) {
	switch (channel) {
	case I2C_0:
		if (TRANSMITTER_MODE == tx_or_rx) {
			I2C0->C1 |= I2C_C1_TX_MASK; //Set as trasmitter 1
		} else {
			I2C0->C1 &= ~(I2C_C1_TX_MASK); //Set as Receiver 0
		}
		break;
	case I2C_1:
		if (TRANSMITTER_MODE == tx_or_rx) {
			I2C1->C1 |= I2C_C1_TX_MASK; //Set as trasmitter 1
		} else {
			I2C1->C1 &= ~(I2C_C1_TX_MASK); //Set as Receiver 0
		}
		break;
	case I2C_2:
		if (TRANSMITTER_MODE == tx_or_rx) {
			I2C2->C1 |= I2C_C1_TX_MASK; //Set as trasmitter 1
		} else {
			I2C2->C1 &= ~(I2C_C1_TX_MASK); //Set as Receiver 0
		}
		break;
	default:
		break;
	}

}

void I2C_repeted_start(i2c_channel_t channel) {

	switch (channel) {
	case I2C_0:
		I2C0->C1 |= I2C_C1_RSTA_MASK; //writes 1 in the RSTA bit generating a  repeated START condition
		break;
	case I2C_1:
		I2C1->C1 |= I2C_C1_RSTA_MASK;
		break;
	case I2C_2:
		I2C2->C1 |= I2C_C1_RSTA_MASK;
		break;
	default:
		break;
	}
	//I2C_C1_MST(0b1); //Preguntar sobre usar esto
}

/*!
 \brief
 It reads data from the receiving buffer.
 \return void
 */
uint8_t I2C_read_byte(i2c_channel_t channel) {

	uint8_t byte_value = FALSE;
	switch (channel) {
	case I2C_0:
		byte_value = I2C0->D;
		break;
	case I2C_1:
		byte_value = I2C1->D;
		break;
	case I2C_2:
		byte_value = I2C2->D;
		break;
	default:
		break;
	}

	return (byte_value); //Retorna el valor del buffer
}

uint8_t I2C_get_ack(i2c_channel_t channel) {
	uint8_t ack_state = FALSE; //0 received, 1 not received

	switch (channel) {
	case I2C_0:
		ack_state = (I2C0->S & I2C_S_RXAK_MASK);
		break;
	case I2C_1:
		ack_state = (I2C1->S & I2C_S_RXAK_MASK);
		break;
	case I2C_2:
		ack_state = (I2C2->S & I2C_S_RXAK_MASK);
		break;
	default:
		break;
	}

	return (ack_state);
}

void I2C_stop(i2c_channel_t channel) {

	switch (channel) {
	case I2C_0:
		I2C0->C1 &= ~I2C_C1_TXAK_MASK; /* An acknowledge signal is sent to the bus on the following receiving byte */
		break;
	case I2C_1:
		I2C1->C1 &= ~I2C_C1_TXAK_MASK; /* An acknowledge signal is sent to the bus on the following receiving byte */
		break;
	case I2C_2:
		I2C2->C1 &= ~I2C_C1_TXAK_MASK; /* An acknowledge signal is sent to the bus on the following receiving byte */
		break;
	default:
		break;
	}
	/** The mode of operation changes from master to slave */
	I2C_mst_or_slv_mode(channel, SLAVE); // When MST bit of C1 register changes from 1 to 0, a STOP signal is generated
	/** I2C is change to receiver mode */
	I2C_tx_rx_mode(channel, RECEIVER_MODE); //Changes to receiver mode
}
