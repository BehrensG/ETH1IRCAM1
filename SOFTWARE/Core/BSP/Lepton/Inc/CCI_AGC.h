/*
 * CCI_AGC.h
 *
 *  Created on: Feb 15, 2024
 *      Author: grzegorz
 */

#ifndef BSP_LEPTON_INC_CCI_AGC_H_
#define BSP_LEPTON_INC_CCI_AGC_H_

#include "CCI.h"


typedef enum
{
	CCI_AGC_DISABLE,
	CCI_AGC_ENABLE

} cci_agc_enable_e;

typedef enum
{
	CCI_AGC_LINEAR=0,
	CCI_AGC_HEQ

}cci_agc_policy_e;


typedef struct
{
	uint16_t startCol;
	uint16_t startRow;
	uint16_t endCol;
	uint16_t endRow;

}cci_agc_roi_t;

#pragma pack(push, 1)

// size 8
typedef struct
{
	uint16_t startCol;
	uint16_t startRow;
	uint16_t endCol;
	uint16_t endRow;

}cci_agc_roi_struct_t;

typedef union
{
	struct
	{
		cci_agc_roi_t roi;

	}structure;

	uint16_t uint16[4];

}cci_agc_roi_union_t;

#pragma pack(pop)

typedef struct
{
	uint16_t minIntensity;
	uint16_t maxIntensity;
	uint16_t meanIntensity;
	uint16_t numPixels;

}cci_agc_histogram_statistics_t;


typedef enum
{
	CCI_AGC_SCALE_TO_8_BITS = 0,
	CCI_AGC_SCALE_TO_14_BITS

}cci_agc_heq_scale_factor_e;


cci_result_t CCI_SetAgcEnableState(cci_agc_enable_e state);

#endif /* BSP_LEPTON_INC_CCI_AGC_H_ */
