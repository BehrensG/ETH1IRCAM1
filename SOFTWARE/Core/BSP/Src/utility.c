/*
 * utility.c
 *
 *  Created on: Feb 15, 2024
 *      Author: grzegorz
 */

#include "utility.h"

inline bool UTIL_Timeout(uint32_t start, uint32_t timeout)
{
	if (((HAL_GetTick() - start) >= timeout) || (0U == timeout))
	{
		return false;
	}
	else
	{
		return true;
	}
}

#define RNG_HTCFG_1   0x17590ABCU /* Magic number */
#define RNG_HTCFG     0x00007274U /* For best latency and to be compliant with NIST */

uint32_t UTIL_RandomNumbersGeneration(void)
{
	uint32_t start = HAL_GetTick();
	uint32_t rng = 0;

	LL_RNG_EnableCondReset(RNG);
	LL_RNG_SetHealthConfig(RNG,RNG_HTCFG_1);
	LL_RNG_SetHealthConfig(RNG,RNG_HTCFG);
	LL_RNG_DisableCondReset(RNG);

	/* Initialize random numbers generation */
	LL_RNG_Enable(RNG);

	while (!LL_RNG_IsActiveFlag_DRDY(RNG))
	{
		if(!UTIL_Timeout(start, 100))
			break;
	}

	/* Check if error occurs */
	if (   (LL_RNG_IsActiveFlag_CECS(RNG) )
		|| (LL_RNG_IsActiveFlag_SECS(RNG))  )
	{
	}

	/* Otherwise, no error detected : Value of generated random number could be retrieved
	   and stored in dedicated array */
	rng = LL_RNG_ReadRandData32(RNG);


	/* Stop random numbers generation */
	LL_RNG_Disable(RNG);

	return rng;
}

void Configure_CRC(void)
{

  /* (1) Enable peripheral clock for CRC                   *********************/
  LL_AHB4_GRP1_EnableClock(LL_AHB4_GRP1_PERIPH_CRC);

  /* (2) Configure CRC functional parameters  ********************************/

  /* Configure CRC calculation unit with default generating polynomial, 32-bit */
  /* Reset value is LL_CRC_DEFAULT_CRC32_POLY */
  // LL_CRC_SetPolynomialCoef(CRC, LL_CRC_DEFAULT_CRC32_POLY);
  /* Reset value is LL_CRC_POLYLENGTH_32B */
  // LL_CRC_SetPolynomialSize(CRC, LL_CRC_POLYLENGTH_32B);

  /* Initialize default CRC initial value */
  /* Reset value is LL_CRC_DEFAULT_CRC_INITVALUE */
  // LL_CRC_SetInitialData(CRC, LL_CRC_DEFAULT_CRC_INITVALUE);

  /* Set input data inversion mode : No inversion*/
  /* Reset value is LL_CRC_INDATA_REVERSE_NONE */
  // LL_CRC_SetInputDataReverseMode(CRC, LL_CRC_INDATA_REVERSE_NONE);

  /* Set output data inversion mode : No inversion */
  /* Reset value is LL_CRC_OUTDATA_REVERSE_NONE */
  // LL_CRC_SetOutputDataReverseMode(CRC, LL_CRC_OUTDATA_REVERSE_NONE);
}

/**
  * @brief  This function performs CRC calculation on buffer_size bytes from input data buffer data_buffer.
  * @param  buffer_size Nb of bytes to be processed for CRC calculation
  * @retval 32-bit CRC value computed on input data buffer
  */
uint32_t UTIL_CalculateCRC(uint8_t* data_buffer, uint32_t buffer_size)
{
  register uint32_t data = 0;
  register uint32_t index = 0;

  /* Compute the CRC of Data Buffer array*/
  for (index = 0; index < (buffer_size / 4); index++)
  {
    data = (uint32_t)((data_buffer[4 * index] << 24) | (data_buffer[4 * index + 1] << 16) | (data_buffer[4 * index + 2] << 8) | data_buffer[4 * index + 3]);
    LL_CRC_FeedData32(CRC, data);
  }

  /* Last bytes specific handling */
  if ((buffer_size % 4) != 0)
  {
    if  (buffer_size % 4 == 1)
    {
      LL_CRC_FeedData8(CRC, data_buffer[4 * index]);
    }
    if  (buffer_size % 4 == 2)
    {
      LL_CRC_FeedData16(CRC, (uint16_t)((data_buffer[4 * index]<<8) | data_buffer[4 * index + 1]));
    }
    if  (buffer_size % 4 == 3)
    {
      LL_CRC_FeedData16(CRC, (uint16_t)((data_buffer[4 * index]<<8) | data_buffer[4 * index + 1]));
      LL_CRC_FeedData8(CRC, data_buffer[4 * index + 2]);
    }
  }

  /* Return computed CRC value */
  return(LL_CRC_ReadData32(CRC));
}

