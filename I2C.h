/**
	\file
	\brief
		This is the header file for the I2C device driver.
		i.e., this is the application programming interface (API) for the GPIO peripheral.
	\author J. Luis Pizano Escalante, luispizano@iteso.mx
	\date	7/09/2018
	\todo
	    Functions are not implemented.
 */

#ifndef I2C_H_
#define I2C_H_

#include "MK64F12.h"
#include "stdint.h"
#include "GPIO.h"
#include "Bits.h"


#define I2C0_CG 0x40 //SCGC4
#define I2C1_CG 0x80 //SCGC4
#define I2C2_CG 0x40 //SCGC1
#define SYSTEM_CLOCK_RATE 21000000U
#define I2Cn_F_MULT1_MASK 0
#define I2Cn_F_MULT2_MASK 0x40
#define I2Cn_F_MULT4_MASK 0x80
#define I2Cn_F_MULT2_VALUE 2
#define MCP7940M_RTC_BAUD_RATE (400000U)
#define I2C_F_ICR3_VALUE 0x3 //ICR value for SCL_div of 26, SCL_div is 26.25 for 400KHz with mult of 2
#define I2Cn_C1_MST_MASK 0x20
#define I2Cn_C1_TXAK_MASK 0x8
#define I2Cn_SMB_FACK_MASK (0x80) //NACK/ACK on this Byte instead of the following
#define I2Cn_C1_IICEN_MASK 0x80 //Yes, 0x80 again.
//#define I2C_S_BUSY_SHIFT 5U
/**
 * \brief This enum define the I2C channel to be used.
 */
typedef enum {I2C_0, I2C_1, I2C_2} i2c_channel_t;
typedef enum {SLAVE, MASTER} master_or_slave_t;
typedef enum {RECEIVER_MODE ,TRANSMITTER_MODE} tx_or_rx_mode_t;

typedef struct
{
	i2c_channel_t channel;
	uint8_t ICR_value;
	uint8_t I2C_Enable;
	uint32_t system_clock;
	uint16_t baud_rate;
	uint8_t clock_gating; //Value for I2C_n Clock Gating
	BitsType SDA_pin; //GPIO pin set as I2C SDA
	BitsType SCL_pin; //GPIO pin set as I2C SCL
	gpio_pin_control_register_t* I2C_Pin_config;
	gpio_port_name_t port_name;

}I2C_config_struct_t;




/********************************************************************************************/
 /********************************************************************************************/
 /********************************************************************************************/
 /*!
  	 \brief
  	 	 Configures the I2C port based on the input parameters.
  	 	 Also, internally this function configures the GPIO, pin control register and clock gating, to be used as I2C.
  	 	 It is recommended to use pin 2 and 3 of GPIOB.
  	 \param[in] channel It is the channel to be used.
  	 \param[in] systemClock Frequency of the system.
  	 \param[in] baudRate baud rate between MCU and I2C device.
  	 \return void

  */
void I2C_init(const I2C_config_struct_t* g_I2C_config );//
 /********************************************************************************************/
 /********************************************************************************************/
 /********************************************************************************************/
 /*!
  	 \brief
  	 	 Indicates the status of the bus regardless of slave or master mode. Internally checks the busy bit int the
  	 	 I2Cx_S register. This bit is set when a START signal is detected and cleared when a STOP signal is detected.
  	 \return This function returns a 0 logic if the bus is idle and returns 1 logic if the bus is busy.

  */
 uint8_t I2C_busy(i2c_channel_t channel);
 /********************************************************************************************/
 /********************************************************************************************/
 /********************************************************************************************/
 /*!
  	 \brief
  	 	 It selects between master or slave mode.
  	 \param[in] masterOrSlave If == 1 master mode, if == 0 slave mode.
  	 \return void

  */
 void I2C_mst_or_slv_mode(i2c_channel_t channel,master_or_slave_t mst_or_slv);//
 /********************************************************************************************/
 /********************************************************************************************/
 /********************************************************************************************/
 /*!
  	 \brief
  	 	 It selects between transmitter mode or receiver mode.
  	 \param[in] txOrRx If == 1 transmitter mode, if == 0 slave mode.
  	 \return void

  */
 void I2C_tx_rx_mode(i2c_channel_t channel,tx_or_rx_mode_t tx_or_rx);
 /********************************************************************************************/
 /********************************************************************************************/
 /********************************************************************************************/
 /*!
  	 \brief
  	 	 It generates the Not ACKnowledge that is needed when the master reads data.
  	 \return void

  */
 void I2C_nack(i2c_channel_t channel);//
 /********************************************************************************************/
 /********************************************************************************************/
 /********************************************************************************************/
 /*!
  	 \brief
  	 	 It generates a repeated start that is needed when master reads data.
  	 \return void

  */
 void I2C_repeted_start(i2c_channel_t channel);
 /********************************************************************************************/
 /********************************************************************************************/
 /********************************************************************************************/
 /*!
  	 \brief
  	 	 It writes the data to be transmitted into the transmission buffer. When you want to
  	 	 write a value into the buffer you need to use this sentence I2C0_D = data. Avoid to use
  	 	 masks like this I2C0_D |= data.
  	 \return void

  */
void I2C_write_byte(i2c_channel_t channel,uint8_t data);//
/********************************************************************************************/
/********************************************************************************************/
/********************************************************************************************/
/*!
 	 \brief
 	 	 It reads data from the receiving buffer.
 	 \return void

 */
uint8_t  I2C_read_byte(i2c_channel_t channel);
/********************************************************************************************/
/********************************************************************************************/
/********************************************************************************************/
/*!
 	 \brief
 	 	 Indicates the status of the bus regardless of slave or master mode. Internally checks the interrupt flag of the
 	 	 I2Cx_S register. This bit is set on the following events:
			• One byte transfer, including ACK/NACK bit, completes if FACK is 0. An ACK or NACK is sent on the
			  bus by writing 0 or 1 to TXAK after this bit is set in receive mode.
			• One byte transfer, excluding ACK/NACK bit, completes if FACK is 1.

		This function should be implemented as a blocking function by using  while((I2C0->S & 0x02)== 0);, the bit number 2 of this register must be set.
		The blocking implementation of this function only to reduce the complexity of the lab. However, blocking implementation must be avoided.
 	 \return Void.

 */
void I2C_wait(i2c_channel_t channel);//
/********************************************************************************************/
/********************************************************************************************/
/********************************************************************************************/
/*!
 	 \brief
 	 	 Indicates if the acknowledge was received.
 	 \return This function returns a 0 logic if the acknowledge was received and returns 1 logic if the acknowledge was not received.

 */
uint8_t I2C_get_ack(i2c_channel_t channel);
/********************************************************************************************/
/********************************************************************************************/
/********************************************************************************************/
/*!
 	 \brief
 	 	 Generates the start signal. When MST bit is changed from 0 to 1, a START signal is generated 
 	 	 on the bus and master mode is selected. Also, inside the function the I2C is
 	 	 change to transmitter mode.
 	 \return void

 */
void I2C_start(i2c_channel_t channel);//
/********************************************************************************************/
/********************************************************************************************/
/********************************************************************************************/
/*!
 	 \brief
 	 	 Generates the stop signal. When this bit changes from 1 to 0, a STOP signal is generated 
 	 	 and the mode of operation changes from master to slave. Also, inside the function the I2C is
 	 	 change to receiver mode.
 	 \return void

 */
void I2C_stop(i2c_channel_t channel);

#endif /* I2C_H_ */
