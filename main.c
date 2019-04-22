/**
 \file
 \brief
 This project shows how works the emulation of a VT 100 in teraterm.
 \author J. Luis Pizano Escalante, luispizano@iteso.mx
 \date	05/03/2019
 \todo
 To implement the UART_init function
 */

#include "MK64F12.h" /* include peripheral declarations */
#include "UART.h"/**UART device driver*/
#include "NVIC.h"/**NVIC device driver*/
#include "GPIO.h"
#include "UART_time.h"
#include "I2C.h"
#include "Delay.h"
#include "PIT.h"
#include "Matrcial_LEDs.h"
#include "UART_string.h"

#define DEBUG
#define DELAY_TIME 1000



#ifdef DEBUG
#include <stdio.h>
#endif


uint8_t numeros[30] = "Quiero mis Vacaciones ";

/**This is mail box to received the information from the serial port*/
extern uart_mail_box_t g_mail_box_uart_0;

gpio_pin_control_register_t g_I2C_pin_Conf = PORT_PCR_MUX(0x2);



//ESTO VA EN EL MAIN****************************************
const I2C_config_struct_t g_I2C_config =				 //*
{														 //*
		I2C_0, //I2C channel							 //*
		I2C_F_ICR3_VALUE, //SCL div = 26 (400KHz) 		 //*
		I2Cn_C1_IICEN_MASK, //I2C enable 				 //*
		SYSTEM_CLOCK_RATE, //SYSTEM CLOCK				 //*
		MCP7940M_RTC_BAUD_RATE, //Baud Rate				 //*
		I2C0_CG,										 //*
		bit_3, //PTB3 as SDA							 //*
		bit_2, //PTB2 as SCL							 //*
		&g_I2C_pin_Conf, //Alt 2 for SDA and SCL		 //*
		GPIO_B											 //*
};														 //*
//***********************************************************


int main(void) {
	//LLenado de arreglos
	UART_setEntryCounter(0);
	UART_setEntry();

	//state machine
	static time_status uart_state = TIME_MENU;
	static ASCII_enter user_entry = ENTER_NULL; //total_input

	//String a desplegar
	uint8_t menu1[] = "1) LEER HORA\r";
	uint8_t menu2[] = "2) CONFIGURAR HORA\r";
	uint8_t cambiarHora[] = "INTRODUCIR HORA ACTUAL EN FORMATO HH:MM:SS\r";
	uint8_t horaActual[] = "LA HORA ACTUAL ES:\r";

	gpio_pin_control_register_t pin_control_register = GPIO_MUX3;

	/**Enables the clock of PortB in order to configures TX and RX of UART peripheral*/
	GPIO_clock_gating(GPIO_B);
	/**Configures the pin control register of pin16 in PortB as UART RX*/
	GPIO_pin_control_register(GPIO_B, 16, &pin_control_register);
	/**Configures the pin control register of pin17 in PortB as UART TX*/
	GPIO_pin_control_register(GPIO_B, 17, &pin_control_register);
	/**Configures UART 0 to transmit/receive at 11520 bauds with a 21 MHz of clock core*/
	UART_init(UART_0, 21000000, BD_115200);

	/**Enables the clock of PortB in order to configures TX and RX of UART peripheral*/
	SIM->SCGC5 = SIM_SCGC5_PORTB_MASK;
	/**Configures the pin control register of pin16 in PortB as UART RX*/
	PORTB->PCR[16] = PORT_PCR_MUX(3);
	/**Configures the pin control register of pin16 in PortB as UART TX*/
	PORTB->PCR[17] = PORT_PCR_MUX(3);
	/**Configures UART 0 to transmit/receive at 11520 bauds with a 21 MHz of clock core*/
	UART_init(UART_0, 21000000, BD_115200);

#ifdef DEBUG
	printf("UART is configured");
#endif
	/**Enables the UART 0 interrupt*/
	UART_interrupt_enable(UART_0);
	/**Enables the UART 0 interrupt in the NVIC*/
	NVIC_enable_interrupt_and_priotity(UART0_IRQ, PRIORITY_10);
	/**The following sentences send strings to PC using the UART_put_string function. Also, the string
	 * is coded with terminal code*/
	NVIC_enable_interrupt_and_priotity(PIT_CH2_IRQ, PRIORITY_6);
	/** VT100 command for text in red and background in cyan*/
	UART_put_string(UART_0, "\033[0;35;43m");
	/*VT100 command for clearing the screen*/
	UART_put_string(UART_0, "\033[2J");
	/** VT100 command for text in red and background in green*/
	UART_put_string(UART_0, "\033[0;32;41m");
	/**Enables interrupts*/
	NVIC_global_enable_interrupts;

	I2C_init(&g_I2C_config);



	//RTCC_ChangeValue(0x80, SECONDS);
	//Matricial_LEDs_init();
	//Matricial_LEDs_clear();
	//Matricial_LEDs_set_string(&numeros);
	//Matricial_LEDs_Modo_Hora_init();
	//Matricial_LEDs_modo_hora();
	RAM_write(STRING_UNO ,&numeros);
	RAM_read_string(STRING_UNO);
	//Matricial_LEDs_put_character(0x003F7FC8C87F3F00, 0x007F7F49493F3600, 0xF9898F);




	for (;;) {
			switch (uart_state) {
			case TIME_MENU:
				//imprime pantalla
				if (FALSE == UART_getEntryCounter()) {
					UART_put_string(UART_0, "\033[10;10H");
					UART_put_string(UART_0, menu1);
					UART_put_string(UART_0, "\033[11;10H");
					UART_put_string(UART_0, menu2);
					UART_put_string(UART_0, "\033[12;10H");
					UART_setEntryCounter(1);
				}
				//revisar buzon
				uart_state = UART_check_buzon(user_entry,uart_state);

				break;
			case TIME_CHANGE:
				//imprime pantalla
				if (FALSE == UART_getEntryCounter()) {
					UART_put_string(UART_0, "\033[10;10H");
					UART_put_string(UART_0, cambiarHora);
					UART_put_string(UART_0, "\033[11;10H");
					UART_setEntryCounter(1);
				}
				//Revisa buzon
				uart_state = UART_check_buzon(user_entry,uart_state);
				break;
			case TIME_ACTUAL:
				//imprime pantalla
				if (FALSE == UART_getEntryCounter()) {
					UART_put_string(UART_0, "\033[10;10H");
					UART_put_string(UART_0, horaActual);
					UART_put_string(UART_0, "\033[11;10H");
					UART_setEntryCounter(1);
				}
				//imprime hora actual
				UART_currentTime();
				//revisa buzon
				uart_state = UART_check_buzon(user_entry,uart_state);
				break;
			}

		}

		return 0;
	}


