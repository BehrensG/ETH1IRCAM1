/*
 * SCPI_LEP_IMG.c
 *
 *  Created on: Feb 22, 2024
 *      Author: BehrensG
 */

#include "SCPI_LEP_IMG.h"
#include "Lepton.h"

extern lep_data_t lepton;


scpi_result_t SCPI_LeptonImageSnapQ(scpi_t * context)
{
	return SCPI_RES_OK;
}

scpi_result_t SCPI_LeptonImageTimeoutQ(scpi_t * context)
{
	SCPI_ResultUInt32(context, lepton.udp_client.timeout);
	return SCPI_RES_OK;
}

scpi_result_t SCPI_LeptonImageTimeout(scpi_t * context)
{
	uint32_t timeout;

	if(!SCPI_ParamUInt32(context, &timeout, TRUE))
	{
		return SCPI_RES_ERR;
	}

	lepton.udp_client.timeout = timeout;

	return SCPI_RES_OK;
}
