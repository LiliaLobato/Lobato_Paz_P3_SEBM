/**
 * \file
 * 		reloj.h
 * \brief
 *		This is the header file for the RTCC control functions needed in the
 *		third lab/practice. It contains all configuration functions and runtime functions needed
 *		to use the RTCC.
 */

#ifndef SOURCES_RELOJ_H_
#define SOURCES_RELOJ_H_


/////////////////////////////////////////////////////////////////////////////////////////////////////////////

//Slave direction
#define ADDRESS_ESCRITURA 0xDE //Escritura
#define ADDRESS_LECTURA 0xDF //Lectura

/**Other macros*/
#define YEARS_INIT_OFFSET 		2000
#define SECS_WITHOUT_ON_RTCC 	0x1F
#define HOURS_WITHOUT_ON_12		0X40
#define HOURS_AMPM	0X20
#define HOURS_AMPM_ON_12 		0x60

/**The following macros are masks and an offset used when converting numbers to ASCII*/
#define ASCII_OFFSET	 	0x30
#define UPPER_NIBBLE_MASK 	0xF0
#define LOWER_NIBBLE_MASK 	0x0F
#define UPPER_NIB_MASK_RTC 	0x70

//DELAY
#define SMALL_DELAY 		3000

/////////////////////////////////////////////////////////////////////////////////////////////////////////////

//DIRECCIONES DE DATOS
typedef enum {
	SECONDS = 0x00,
	MINUTES,
	HOURS,
	WKKD,
	DAYS,
	MONTH,
	YEARS,
	AMPM
} RTCC_RegisterAdresses;

//TIPO DE TIEMPO
typedef enum {
	AM_PM,
	FULL_TIME
} RTCC_Hour_Format;

/////////////////////////////////////////////////////////////////////////////////////////////////////////////

//Regresa un registro específico
uint8_t RTCC_GetValue(RTCC_RegisterAdresses RTCC_Reg);

//Cambia el valor de un registro
void RTCC_ChangeValue(uint8_t value, RTCC_RegisterAdresses RTCC_Reg);

//Cambia la fecha u hora
void RTCC_setGlobalVariable(RTCC_RegisterAdresses variable, uint32_t value);

//Lee el tiempo y fecha
uint32_t RTCC_getGlobalVariable(RTCC_RegisterAdresses variable);

//Lee el registro de tiempo completo
void RTCC_ReadDateTimeFull(void);

//Convertir BCD a binario
uint8_t convertBCD_toBinary(uint8_t value);

#endif /* SOURCES_RELOJ_H_ */
