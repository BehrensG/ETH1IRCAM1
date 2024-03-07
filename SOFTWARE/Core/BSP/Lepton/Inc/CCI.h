/*
 * CCI.h
 *
 *  Created on: Feb 14, 2024
 *      Author: grzegorz
 */

#ifndef BSP_LEPTON_CCI_H_
#define BSP_LEPTON_CCI_H_

#include "BSP.h"

// Device characteristics
#define CCI_WORD_LENGTH 0x02
#define CCI_ADDRESS 0x2A

// CCI register locations
#define CCI_REG_STATUS 0x0002
#define CCI_REG_COMMAND 0x0004
#define CCI_REG_DATA_LENGTH 0x0006
#define CCI_REG_DATA_0 0x0008
#define CCI_REG_DATA_1 0x000A
#define CCI_REG_DATA_2 0x000C
#define CCI_REG_DATA_3 0x000E
#define CCI_REG_DATA_4 0x0010
#define CCI_REG_DATA_5 0x0012
#define CCI_REG_DATA_6 0x0014
#define CCI_REG_DATA_7 0x0016
#define CCI_REG_DATA_8 0x0018
#define CCI_REG_DATA_9 0x001A
#define CCI_REG_DATA_10 0x001C
#define CCI_REG_DATA_11 0x001E
#define CCI_REG_DATA_12 0x0020
#define CCI_REG_DATA_13 0x0022
#define CCI_REG_DATA_14 0x0024
#define CCI_REG_DATA_15 0x0026
#define CCI_BLOCK_BUF_0 0xF800
#define CCI_BLOCK_BUF_1 0xFC00




#define CCI_BOOT_STATUS	0x04
#define CCI_BOOT_MODE	0x02

#define CCI_BOOT_OK		0x06
#define CCI_NOT_BUSY	0x06
#define CCI_BUSY		0x07

typedef union
{
	uint16_t uint16;
	uint8_t uint8[2];
}cci_union_data_t;

typedef enum
{
	CCI_OK 							= 0,
	CCI_COMM_OK						= CCI_OK,
	CCI_ERROR						= -1,
	CCI_NOT_READY					= -2,
	CCI_RANGE_ERROR					= -3,
	CCI_CHECKSUM_ERROR				= -4,
	CCI_BAD_ARG_POINTER_ERROR		= -5,
	CCI_DATA_SIZE_ERROR				= -6,
	CCI_UNDEFINED_FUNCTION_ERROR	= -7,
	CCI_FUNCTION_NOT_SUPPORTED		= -8,
	CCI_DATA_OUT_OF_RANGE_ERROR		= -9,
	CCI_COMMAND_NOT_ALLOWED			= -10,

	/* OTP access errors */
	CCI_OTP_WRITE_ERROR				= -15,
	CCI_OTP_READ_ERROR				= -16,
	CCI_OTP_NOT_PROGRAMMED_ERROR	= -18,

	/* I2C Errors */
	CCI_ERROR_I2C_BUS_NOT_READY		= -20,
	CCI_ERROR_I2C_BUFFER_OVERFLOW	= -22,
	CCI_ERROR_I2C_ARBITRATION_LOST	= -23,
	CCI_ERROR_I2C_BUS_ERROR			= -24,
	CCI_ERROR_I2C_NACK_RECEIVED		= -25,
	CCI_ERROR_I2C_FAIL				= -26,

	/* Processing Errors */
	CCI_DIV_ZERO_ERROR				= -80,

	/* Comm Errors */
	CCI_COMM_PORT_NOT_OPEN			= -101,
	CCI_COMM_INVALID_PORT_ERROR		= -102,
	CCI_COMM_RANGE_ERROR			= -103,
	CCI_ERROR_CREATING_COMM			= -104,
	CCI_ERROR_STARTING_COMM			= -105,
	CCI_ERROR_CLOSING_COMM			= -106,
	CCI_COMM_CHECKSUM_ERROR			= -107,
	CCI_COMM_NO_DEV					= -108,
	CCI_TIMEOUT_ERROR				= -109,
	CCI_COMM_ERROR_WRITING_COMM		= -110,
	CCI_COMM_ERROR_READING_COMM		= -111,
	CCI_COMM_COUNT_ERROR			= -112,

	/* Other Errors */
	CCI_OPERATION_CANCELED			= -126,
	CCI_UNDEFINED_ERROR_CODE		= -127

} cci_result_t;


cci_result_t CCI_ReadBootStatus(uint8_t* boot_status);
cci_result_t CCI_ReadRegister(uint16_t address, cci_union_data_t* readout);
cci_result_t CCI_WriteRegister(const uint16_t address, const uint16_t value);
cci_result_t CCI_WaitBusyClear(void);
cci_result_t CCI_ReadCMD(uint16_t cmd, uint16_t size, cci_union_data_t* readout);
cci_result_t CCI_WriteCMD(uint16_t cmd, uint16_t size, cci_union_data_t* value);

#endif /* BSP_LEPTON_CCI_H_ */
