/*
 * CCI.c
 *
 *  Created on: Feb 14, 2024
 *      Author: grzegorz
 */

#include "CCI.h"
#include "main.h"
#include "utility.h"

cci_result_t CCI_ReadRegister(uint16_t address, cci_union_data_t *readout)
{

	cci_union_data_t reg;

	reg.uint16 = __REVSH(address);

	if (HAL_OK
			!= HAL_I2C_Master_Transmit(&hi2c4, (CCI_ADDRESS << 1), reg.uint8,
					CCI_WORD_LENGTH, 1000))
	{
		return CCI_ERROR_I2C_BUS_ERROR;
	}

	if (HAL_OK
			!= HAL_I2C_Master_Receive(&hi2c4, (CCI_ADDRESS << 1 | 0x01),
					readout->uint8, CCI_WORD_LENGTH, 1000))
	{
		return CCI_ERROR_I2C_BUS_ERROR;
	}

	return CCI_OK;
}

cci_result_t CCI_WriteRegister(const uint16_t address, const uint16_t value)
{
	uint8_t uint8[4] =
	{ address >> 8 & 0xFF, address & 0xFF, value >> 8 & 0xFF, value & 0xFF };

	if (HAL_OK
			!= HAL_I2C_Master_Transmit(&hi2c4, (CCI_ADDRESS << 1), uint8, 4,
					1000))
	{
		return CCI_ERROR_I2C_BUS_ERROR;
	}

	return CCI_OK;

}

cci_result_t CCI_WaitBusyClear(void)
{
	uint32_t start = HAL_GetTick();
	cci_union_data_t readout;
	cci_result_t status = CCI_OK;

	do
	{

		status = CCI_ReadRegister(CCI_REG_STATUS, &readout);

		if (!UTIL_Timeout(start, 5000))
		{
			status = CCI_TIMEOUT_ERROR;
			break;
		}

	} while ( CCI_NOT_BUSY != readout.uint8[1]);

	if (CCI_OK == status)
	{

		return (int8_t) readout.uint8[0];

	}
	else
	{
		return status;
	}

}

cci_result_t CCI_ReadBootStatus(uint8_t *boot_status)
{
	cci_union_data_t rx;

	CCI_ReadRegister(CCI_REG_STATUS, &rx);

	*boot_status = rx.uint8[1];

	return CCI_OK;
}


cci_result_t CCI_ReadCMD(uint16_t cmd, uint16_t size, cci_union_data_t* readout)
{
	cci_result_t status = CCI_OK;

	cci_union_data_t rx[16];

	CCI_WriteRegister(CCI_REG_DATA_LENGTH, size);
	CCI_WriteRegister(CCI_REG_COMMAND, cmd);

	status = CCI_WaitBusyClear();

	if (CCI_OK != status)
		return status;

	for (uint8_t x = 0; x < size; x++)
	{

		status = CCI_ReadRegister(CCI_REG_DATA_0 + x * CCI_WORD_LENGTH, &rx[x]);

		if (CCI_OK != status)
			break;

		(readout + x)->uint16 = __REVSH(rx[x].uint16);

	}

	return status;
}

cci_result_t CCI_WriteCMD(uint16_t cmd, uint16_t size, cci_union_data_t* value)
{
	for (uint8_t x = 0; x < size; x++)
	{
		if(CCI_OK != CCI_WriteRegister(CCI_REG_DATA_0 + 2*x, (value + x)->uint16))
			return CCI_ERROR;
	}

	CCI_WriteRegister(CCI_REG_DATA_LENGTH, size);
	CCI_WriteRegister(CCI_REG_COMMAND, cmd);

	return CCI_WaitBusyClear();
}
