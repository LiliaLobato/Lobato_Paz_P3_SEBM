/**
 * \file
 * 		reloj.c
 * \brief
 *		This is the source file for the RTCC driver for the third lab/practice.
 *		It contains all the implementation for configuration functions and runtime functions.
 * 		needed to use the RTCC.
 */

#include "I2C.h"
#include "reloj.h"
#include "Delay.h"

/////////////////////////////////////////////////////////////////////////////////////////////////////////////

//Guarda la fecha actual
static struct {
	uint32_t currentYear;
	uint32_t currentMonth;
	uint32_t currentDay;
	uint32_t currentHour;
	uint32_t currentAMPM;
	uint32_t currentMinute;
	uint32_t currentSecond;
} RTCC_DateTimeVariables;

/////////////////////////////////////////////////////////////////////////////////////////////////////////////

//cambia las variables globales de tiempo
void RTCC_setGlobalVariable(RTCC_RegisterAdresses variable, uint32_t value) {
	switch (variable) { /**The variable which is to be set is checked*/
	case SECONDS: /**SECONDS is selected*/
		RTCC_DateTimeVariables.currentSecond = value;
		break;
	case MINUTES: /**MINUTES is selected*/
		RTCC_DateTimeVariables.currentMinute = value;
		break;
	case HOURS: /**HOURS is selected*/
		RTCC_DateTimeVariables.currentHour = value;
		break;
	case DAYS: /**DAYS is selected*/
		RTCC_DateTimeVariables.currentDay = value;
		break;
	case MONTH: /**MONTH is selected*/
		RTCC_DateTimeVariables.currentMonth = value;
		break;
	case YEARS: /**YEARS is selected*/
		RTCC_DateTimeVariables.currentYear = value;
		break;
	case AMPM:
		RTCC_DateTimeVariables.currentAMPM = value;
		break;
	default: /**Invalid option*/
		break; /**No actions taken*/
	}
}

//Retorna un valor de fecha
uint32_t RTCC_getGlobalVariable(RTCC_RegisterAdresses variable) {
	switch (variable) { /**The variable which is to be set is checked*/
	case SECONDS: /**SECONDS is selected*/
		return RTCC_DateTimeVariables.currentSecond;
		break;
	case MINUTES: /**MINUTES is selected*/
		return RTCC_DateTimeVariables.currentMinute;
		break;
	case HOURS: /**HOURS is selected*/
		return RTCC_DateTimeVariables.currentHour;
		break;
	case DAYS: /**DAYS is selected*/
		return RTCC_DateTimeVariables.currentDay;
		break;
	case MONTH: /**MONTH is selected*/
		return RTCC_DateTimeVariables.currentMonth;
		break;
	case YEARS: /**YEARS is selected*/
		return RTCC_DateTimeVariables.currentYear;
		break;
	case AMPM:
		return RTCC_DateTimeVariables.currentAMPM;
		break;
	default: /**Invalid option*/
		return FALSE; /**FALSE is returned*/
		break;
	}
}

//Lectura ed todos los registros del MCP7940M
void RTCC_ReadDateTimeFull(void) {
	/**Reading the SECONDS*/
	RTCC_DateTimeVariables.currentSecond = convertBCD_toBinary(
			RTCC_GetValue(SECONDS));
	/**Reading the MINUTES*/
	RTCC_DateTimeVariables.currentMinute = convertBCD_toBinary(
			RTCC_GetValue(MINUTES));
	/**Reading the AM/PM**/
	RTCC_DateTimeVariables.currentAMPM = convertBCD_toBinary(
			(RTCC_GetValue(HOURS) & HOURS_AMPM) >> 5);
	/**Reading the HOURS*/
	if (FALSE == RTCC_DateTimeVariables.currentAMPM & 0X40 >> 6) {
		RTCC_DateTimeVariables.currentHour = convertBCD_toBinary(
				RTCC_GetValue(HOURS) & ~(HOURS_WITHOUT_ON_12));
	} else {
		uint32_t horas = convertBCD_toBinary(RTCC_GetValue(HOURS) & ~(HOURS_WITHOUT_ON_12));
		uint32_t horasL = horas-((uint32_t)(horas/10)*10);
		if (RTCC_getGlobalVariable(HOURS) / 10) {
			RTCC_DateTimeVariables.currentHour = 10+horasL;
		} else {
			RTCC_DateTimeVariables.currentHour = 00+horasL;
		}
	}

	/**Reading the DAYS*/
	RTCC_DateTimeVariables.currentDay = convertBCD_toBinary(
			RTCC_GetValue(DAYS));
	/**Reading the MONTH*/
	RTCC_DateTimeVariables.currentMonth = convertBCD_toBinary(
			RTCC_GetValue(MONTH) & SECS_WITHOUT_ON_RTCC);
	/**Reading the SECONDS*/
	RTCC_DateTimeVariables.currentYear = convertBCD_toBinary(
			RTCC_GetValue(YEARS));
}

//Lee un registro del MCP7940M
uint8_t RTCC_GetValue(RTCC_RegisterAdresses RTCC_Reg) {

	delay(DELAY);

	uint8_t dataFromRTCC;
	/**Configuring the I2C in TX mode and Master mode*/
	I2C_start(I2C_0);

	/**Writing RTCC address in the data register*/
	I2C_write_byte(I2C_0, ADDRESS_ESCRITURA);
	/**Checking if the I2C module is busy by checking the transfer complete flag (technically interrupt pending flag)*/
	I2C_wait(I2C_0);
	/**Waiting for acknowledge.This function is able to detect if an ACK was received*/
	I2C_get_ack(I2C_0);

	/**Writing the specified register address in the data register*/
	I2C_write_byte(I2C_0, RTCC_Reg);
	/**Checking if the I2C module is busy by checking the transfer complete flag (technically interrupt pending flag)*/
	I2C_wait(I2C_0);
	/**Waiting for acknowledge.This function is able to detect if an ACK was received*/
	I2C_get_ack(I2C_0);

	/**Generating a new start*/
	I2C_repeted_start(I2C_0);
	/**Writing RTCC address plus 1 in the data register*/
	I2C_write_byte(I2C_0, ADDRESS_LECTURA);
	/**Checking if the I2C module is busy by checking the transfer complete flag (technically interrupt pending flag)*/
	I2C_wait(I2C_0);
	/**Waiting for acknowledge.This function is able to detect if an ACK was received*/
	I2C_get_ack(I2C_0);

	/**Changing the I2C module to receiver mode*/
	I2C_tx_rx_mode(I2C_0, RECEIVER_MODE);

	/**Dummy read*/
	dataFromRTCC = I2C_read_byte(I2C_0);

	/**Checking if the I2C module is busy by checking the transfer complete flag (technically interrupt pending flag)*/
	I2C_wait(I2C_0);

	/**Generating not acknowledge*/
	I2C_nack(I2C_0);

	/**Generating stop signal*/
	I2C_stop(I2C_0);
	/**Reading the true value*/
	dataFromRTCC = I2C_read_byte(I2C_0);
	/**Returning the value*/
	return (dataFromRTCC);
}

//Cambia el valor de un registro
void RTCC_ChangeValue(uint8_t value, RTCC_RegisterAdresses RTCC_Reg) {

	/**A small delay is executed, which will ensure that no problems will happen with the I2C device communication
	 * when multiple reads/writes are executed one after another*/
	delay(DELAY);

	/**Configuring the I2C in TX mode and Master mode*/
	I2C_start(I2C_0);

	/**Writing RTCC address in the data register*/
	I2C_write_byte(I2C_0, ADDRESS_ESCRITURA);
	/**Checking if the I2C module is busy by checking the transfer complete flag (technically interrupt pending flag)*/
	I2C_wait(I2C_0);
	/**Waiting for acknowledge.This function is able to detect if an ACK was received*/
	I2C_get_ack(I2C_0);

	/**Writing the specified register address in the data register*/
	I2C_write_byte(I2C_0, RTCC_Reg);
	/**Checking if the I2C module is busy by checking the transfer complete flag (technically interrupt pending flag)*/
	I2C_wait(I2C_0);
	/**Waiting for acknowledge.This function is able to detect if an ACK was received*/
	I2C_get_ack(I2C_0);

	/**Writing the register with the specified value*/
	I2C_write_byte(I2C_0, value);
	/**Checking if the I2C module is busy by checking the transfer complete flag (technically interrupt pending flag)*/
	I2C_wait(I2C_0);
	/**Waiting for acknowledge.This function is able to detect if an ACK was received*/
	I2C_get_ack(I2C_0);

	/**Generating stop signal*/
	I2C_stop(I2C_0);
}

//Conversion BCD to binary
uint8_t convertBCD_toBinary(uint8_t value) {
	return (((value & UPPER_NIB_MASK_RTC) >> 4) * 10)
			+ (value & LOWER_NIBBLE_MASK);
}
