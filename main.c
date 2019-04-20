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
#include "UART_string.h"
#include "I2C.h"
#include "Delay.h"

#define DEBUG
#define DELAY_TIME 1000
#define CONECTADO 0
#define DESCONECTADO 1

#ifdef DEBUG
#include <stdio.h>
#endif

/**This is mail box to received the information from the serial port*/
extern uart_mail_box_t g_mail_box_uart_0;

gpio_pin_control_register_t g_I2C_pin_Conf = PORT_PCR_MUX(0x2);

//TABLA CONFIGURACION I2C****************************************
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
		};//*
//***********************************************************

//String a desplegar
static uint8_t menu1[] = "1) CONFIGURAR HORA\r";
static uint8_t menu2[] = "2) CONFIGURAR FECHA\r";
static uint8_t menu3[] = "3) LEER HORA\r";
static uint8_t menu4[] = "4) LEER FECHA\r";
static uint8_t menu5[] = "5) ESCRIBIR MENSAJE EN MEMORIA I2C\r";
static uint8_t menu6[] = "6) MOSTRAR HORA EN MATRIZ DE LEDS\r";
static uint8_t menu7[] = "7) MOSTRAR MENASAJE EN MATRIZ DE LEDS\r";
static uint8_t statusReloj[] = "STATUS RELOJ = ";
static uint8_t cambiarHora[] = "INTRODUCIR HORA EN FORMATO HH:MM:SS (24HRS)\r";
static uint8_t cambiarFecha[] = "INTRODUCIR FECHA EN FORMATO DD:MM:AAAA\r";
static uint8_t horaActual[] = "LA HORA ACTUAL ES:\r";
static uint8_t fechaActual[] = "LA FECHA ACTUAL ES:\r";
static uint8_t memoryWrite[] = "NUMERO DE MENSAJE DE 1 A 5:\r";
static uint8_t storeString[] = "ESCRIBE EL MENSAJE:\r";
static uint8_t ledsTime[] =
		"DESEA MOSTRAR LA HORA EN LA MATRIZ DE LEDS? (SI/NO)\r";
static uint8_t ledsMemory1[] = "MENSAJE ALMACENADOS:\r";
static uint8_t ledsMemory2[] = "MENSAJE A MOSTRAR? (1 A 5)\r";

//Estado del reloj
static uint8_t estadoRTCC = CONECTADO;

int main(void) {
	//LLenado de arreglos
	UART_setEntryCounter(0);
	UART_setEntry();

	//state machine
	static time_status uart_state = TIME_MENU;
	static ASCII_enter user_entry = ENTER_NULL; //total_input

	gpio_pin_control_register_t pin_control_register = GPIO_MUX3;

	/**Enables the clock of PortB in order to configures TX and RX of UART peripheral*/
	GPIO_clock_gating(GPIO_B);
	/**Configures the pin control register of pin16 in PortB as UART RX*/
	GPIO_pin_control_register(GPIO_B, 16, &pin_control_register);
	/**Configures the pin control register of pin17 in PortB as UART TX*/
	GPIO_pin_control_register(GPIO_B, 17, &pin_control_register);
	/**Configures UART 0 to transmit/receive at 11520 bauds with a 21 MHz of clock core*/
	UART_init(UART_0, 21000000, BD_115200);

	/**Enables the clock of PortC in order to configures TX and RX of UART peripheral*/
	GPIO_clock_gating(GPIO_C);
	/**Configures the pin control register of pin14 in PortC as UART RX*/
	GPIO_pin_control_register(GPIO_C, 14, &pin_control_register);
	/**Configures the pin control register of pin15 in PortC as UART TX*/
	GPIO_pin_control_register(GPIO_C, 15, &pin_control_register);

	/**Configures UART 0 to transmit/receive at 11520 bauds with a 21 MHz of clock core*/
	UART_init(UART_1, 21000000, BD_115200);
	UART_init(UART_4, 21000000, BD_9600);

#ifdef DEBUG
	printf("UART is configured");
#endif

	/**Enables the UART 0 interrupt*/
	UART_interrupt_enable(UART_0);
	/**Enables the UART 0 interrupt in the NVIC*/
	NVIC_enable_interrupt_and_priotity(UART0_IRQ, PRIORITY_10);
	/**Enables the UART 0 interrupt*/
	UART_interrupt_enable(UART_4);
	/**Enables the UART 0 interrupt in the NVIC*/
	NVIC_enable_interrupt_and_priotity(UART4_IRQ, PRIORITY_10);

	//limpia la pantalla
	setScreen();

	/**Enables interrupts*/
	NVIC_global_enable_interrupts;

	I2C_init(&g_I2C_config);

	UART_cleanAllString();
	UART_initRTCC();

	for (;;) {
		//UART_put_char(UART_4, 'i');
		//UART_put_string(UART_4, menu1);

		switch (uart_state) {
		case TIME_MENU:
			//imprime pantalla
			if (FALSE == UART_getEntryCounter()) {
				UART_put_string(UART_0, "\033[10;10H");
				UART_put_string(UART_0, menu1);
				UART_put_string(UART_0, "\033[11;10H");
				UART_put_string(UART_0, menu2);
				UART_put_string(UART_0, "\033[12;10H");
				UART_put_string(UART_0, menu3);
				UART_put_string(UART_0, "\033[13;10H");
				UART_put_string(UART_0, menu4);
				UART_put_string(UART_0, "\033[14;10H");
				UART_put_string(UART_0, menu5);
				UART_put_string(UART_0, "\033[15;10H");
				UART_put_string(UART_0, menu6);
				UART_put_string(UART_0, "\033[16;10H");
				UART_put_string(UART_0, menu7);
				UART_put_string(UART_0, "\033[17;10H");
				UART_put_string(UART_0, statusReloj);
				UART_setEntryCounter(1);
			}
			if (A == convertBCD_toBinary(RTCC_GetValue(SRAM))) {
				UART_put_string(UART_0, "\033[17;25H");
				UART_put_string(UART_0, "CONECTADO    ");
				estadoRTCC = CONECTADO;
			} else {

				UART_put_string(UART_0, "\033[17;25H");
				UART_put_string(UART_0, "DESCONECTADO ");
				estadoRTCC = DESCONECTADO;
			}
			//revisar buzon
			uart_state = UART_check_buzon(user_entry, uart_state);

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
			uart_state = UART_check_buzon(user_entry, uart_state);
			break;
		case DATE_CHANGE:
			//imprime pantalla
			if (FALSE == UART_getEntryCounter()) {
				UART_put_string(UART_0, "\033[10;10H");
				UART_put_string(UART_0, cambiarFecha);
				UART_put_string(UART_0, "\033[11;10H");
				UART_setEntryCounter(1);
			}
			//Revisa buzon
			uart_state = UART_check_buzon(user_entry, uart_state);
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
			if (DESCONECTADO == estadoRTCC) {
				UART_put_string(UART_0, "\033[11;10H");
				UART_put_string(UART_0,
						"RTCC DESCONECTADO, VUELVA A MENU Y RECONECTE");
			} else {
				UART_currentTime();
			}
			//revisa buzon
			uart_state = UART_check_buzon(user_entry, uart_state);
			break;
		case DATE_ACTUAL:
			//imprime pantalla
			if (FALSE == UART_getEntryCounter()) {
				UART_put_string(UART_0, "\033[10;10H");
				UART_put_string(UART_0, fechaActual);
				UART_put_string(UART_0, "\033[11;10H");
				UART_setEntryCounter(1);
			}
			//imprime hora actual
			if (DESCONECTADO == estadoRTCC) {
				UART_put_string(UART_0, "\033[11;10H");
				UART_put_string(UART_0,
						"RTCC DESCONECTADO, VUELVA A MENU Y RECONECTE");
			} else {
				UART_currentDate();
			}
			//revisa buzon
			uart_state = UART_check_buzon(user_entry, uart_state);
			break;
		case MEMORY_WRITE:
			//imprime pantalla
			if (FALSE == UART_getEntryCounter()) {
				UART_put_string(UART_0, "\033[10;10H");
				UART_put_string(UART_0, memoryWrite);
				UART_put_string(UART_0, "\033[11;10H");
				UART_setEntryCounter(1);
			}
			//revisa buzon
			uart_state = UART_check_buzon(user_entry, uart_state);
			break;
		case LEDS_TIME:
			//imprime pantalla
			if (FALSE == UART_getEntryCounter()) {
				UART_put_string(UART_0, "\033[10;10H");
				UART_put_string(UART_0, ledsTime);
				UART_put_string(UART_0, "\033[11;10H");
				UART_setEntryCounter(1);
			}
			//revisa buzon
			uart_state = UART_check_buzon(user_entry, uart_state);
			break;
		case LEDS_MEMORY:
			//imprime pantalla
			if (FALSE == UART_getEntryCounter()) {
				UART_put_string(UART_0, "\033[10;10H");
				UART_put_string(UART_0, ledsMemory1);
				UART_printString();
				UART_put_string(UART_0, ledsMemory2);
				UART_put_string(UART_0, "\033[18;10H");
				UART_setEntryCounter(1);
			}
			//revisa buzon
			uart_state = UART_check_buzon(user_entry, uart_state);
			break;
		case STORE_STRING:
			//imprime pantalla
			if (FALSE == UART_getEntryCounter()) {
				UART_put_string(UART_0, "\033[10;10H");
				UART_put_string(UART_0, storeString);
				UART_put_string(UART_0, "\033[11;10H");
				UART_setEntryCounter(1);
			}
			//revisa buzon
			uart_state = UART_check_buzon(user_entry, uart_state);
			break;
		}
	}
	return 0;
}

