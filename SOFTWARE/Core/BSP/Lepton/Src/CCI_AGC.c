/*
 * CCI_AGC.c
 *
 *  Created on: Feb 15, 2024
 *      Author: grzegorz
 */

#include "CCI_AGC.h"


/* --------------------------------------------------------------------------------------------------------------------
 *
 * AGC Enable and Disable
 *
 * To turn AGC ON is to enable AGC processing. Disabling the AGC will turn the AGC processing OFF and the video
 * data will not be optimized for scene contrast. This command sets and retrieves the AGC state.
 * Note that the Focus metric (see 4.6.3) must be disabled when AGC is enabled.
 *
 * --------------------------------------------------------------------------------------------------------------------
 */

#define CCI_CMD_AGC_GET_ENABLE_STATE 0x0100
#define CCI_CMD_AGC_SET_ENABLE_STATE 0x0101

cci_result_t CCI_GetAgcEnableState(cci_agc_enable_e* agc_enable)
{
	cci_result_t status = CCI_OK;
	cci_union_data_t rx[2];
	uint16_t revsh[2];

	CCI_WriteRegister(CCI_REG_DATA_LENGTH, 2);
	CCI_WriteRegister(CCI_REG_COMMAND, CCI_CMD_AGC_GET_ENABLE_STATE);

	status = CCI_WaitBusyClear();

	if (CCI_OK != status)
		return status;

	for (uint8_t x = 0; x < 2; x++)
	{

		status = CCI_ReadRegister(CCI_REG_DATA_0 + x * CCI_WORD_LENGTH, &rx[x]);

		if (CCI_OK != status)
			break;

		revsh[x] = __REVSH(rx[x].uint16);

	}

	*agc_enable = (cci_agc_enable_e) ((uint32_t) (revsh[1] << 16) | (uint32_t) revsh[0]);

	return CCI_WaitBusyClear();
}

cci_result_t CCI_SetAgcEnableState(cci_agc_enable_e state)
{
	uint32_t value = state;

	CCI_WriteRegister(CCI_REG_DATA_0, value & 0xFFFF);
	CCI_WriteRegister(CCI_REG_DATA_1, value >> 16 & 0xFFFF);
	CCI_WriteRegister(CCI_REG_DATA_LENGTH, 2);
	CCI_WriteRegister(CCI_REG_COMMAND, CCI_CMD_AGC_SET_ENABLE_STATE);

	return CCI_WaitBusyClear();
}

/* --------------------------------------------------------------------------------------------------------------------
 *
 * AGC Policy Select
 *
 * The camera supports 2 AGC policies to process incoming video data, histogram equalization (HEQ) and linear
 * histogram stretch. This command sets and retrieves the AGC policy.
 *
 * --------------------------------------------------------------------------------------------------------------------
 */

#define CCI_CMD_AGC_GET_POLICY_SELECT 0x0104
#define CCI_CMD_AGC_SET_POLICY_SELECT 0x0105

cci_result_t CCI_GetAgcPolicy(cci_agc_policy_e* readout)
{
	cci_result_t status = CCI_OK;
	cci_union_data_t rx[2];

	status = CCI_ReadCMD(CCI_CMD_AGC_GET_POLICY_SELECT, 2, rx);

	if (CCI_OK != status)
		return status;

	*readout = rx[1].uint16 << 16 | rx[0].uint16;

	return CCI_OK;
}

cci_result_t CCI_SetAgcPolicy(cci_agc_policy_e policy)
{
	uint32_t value = policy;
	cci_union_data_t tx[2];

	tx[0].uint16 = value & 0xFFFF;
	tx[1].uint16 = (value >> 16) & 0xFFFF;

	return CCI_WriteCMD(CCI_CMD_AGC_SET_POLICY_SELECT, 2, tx);

}

/* --------------------------------------------------------------------------------------------------------------------
 *
 * AGC ROI Select
 *
 * The AGC algorithms utilize a histogram, which is collected from within a specified rectangular window or
 * Region of Interest (ROI). This region is defined by 4 parameters: start column, start row, end column, and end row.
 * The region is adjustable from full window to a sub-window.
 *
 * --------------------------------------------------------------------------------------------------------------------
 */

#define CCI_CMD_AGC_GET_ROI_SELECT 0x0108
#define CCI_CMD_AGC_SET_ROI_SELECT 0x0109

cci_result_t CCI_GetAgcROI(cci_agc_roi_struct_t* agc_roi)
{
	cci_result_t status = CCI_OK;
	cci_union_data_t rx[4];

	status = CCI_ReadCMD(CCI_CMD_AGC_GET_ROI_SELECT, 4, rx);

	if (CCI_OK != status)
		return status;

	agc_roi->startRow = rx[0].uint16 ;
	agc_roi->startCol = rx[1].uint16 ;
	agc_roi->endRow = rx[2].uint16 ;
	agc_roi->endCol = rx[3].uint16 ;

	return CCI_OK;
}

cci_result_t CCI_SetAgcROI(cci_agc_roi_struct_t agc_roi)
{
	cci_union_data_t tx[4];

	tx[0].uint16 = agc_roi.startRow;
	tx[1].uint16 = agc_roi.startCol;
	tx[2].uint16 = agc_roi.endRow;
	tx[3].uint16 = agc_roi.endCol;

	return CCI_WriteCMD(CCI_CMD_AGC_SET_ROI_SELECT, 4, tx);
}

/* --------------------------------------------------------------------------------------------------------------------
 *
 * AGC Histogram Statistics
 *
 * The AGC algorithms use the image histogram as input. This attribute returns the current Histogram statistics of minimum
 * intensity, maximum intensity, mean intensity, and the number of pixels processed within the defined AGC ROI.
 * This command is Read-only.
 *
 * --------------------------------------------------------------------------------------------------------------------
 */

#define CCI_CMD_AGC_GET_HISTOGRAM_STATISTICS 0x010C

cci_result_t CCI_GetAgcHistogramStatistics(cci_agc_histogram_statistics_t* agc_histogram_statistics)
{
	cci_result_t status = CCI_OK;
	cci_union_data_t rx[4];

	status = CCI_ReadCMD(CCI_CMD_AGC_GET_HISTOGRAM_STATISTICS, 4, rx);

	if (CCI_OK != status)
		return status;

	agc_histogram_statistics->minIntensity = rx[0].uint16 ;
	agc_histogram_statistics->maxIntensity = rx[1].uint16 ;
	agc_histogram_statistics->meanIntensity = rx[2].uint16 ;
	agc_histogram_statistics->numPixels = rx[3].uint16 ;

	return CCI_OK;
}

/* --------------------------------------------------------------------------------------------------------------------
 *
 * AGC HEQ Dampening Factor
 *
 * This parameter is the amount of temporal dampening applied to the HEQ transformation function.
 * An IIR filter of the form (N/256) * previous + ((256-N)/256) * current is applied , and the HEQ dampening factor
 * represents the value N in the equation, i.e., a value that applies to the amount of influence the previous HEQ
 * transformation function has on the current function. . The lower the value of N the higher the influence of the
 * current video frame whereas the higher the value of N the more influence the previous damped transfer function has.
 *
 * --------------------------------------------------------------------------------------------------------------------
 */

#define CCI_CMD_AGC_GET_HEQ_DAMPENING_FACTOR 0x0124
#define CCI_CMD_AGC_SET_HEQ_DAMPENING_FACTOR 0x0125


cci_result_t CCI_GetAgcHeqDampingFactor(uint16_t* agc_heq_damping_factor)
{
	cci_result_t status = CCI_OK;
	cci_union_data_t rx;

	status = CCI_ReadCMD(CCI_CMD_AGC_GET_HEQ_DAMPENING_FACTOR, 1, &rx);

	if (CCI_OK != status)
		return status;

	*agc_heq_damping_factor = rx.uint16;

	return CCI_OK;
}

cci_result_t CCI_SetAgcHeqDampingFactor(uint16_t agc_heq_damping_factor)
{
	cci_union_data_t tx;

	tx.uint16 = agc_heq_damping_factor;

	return CCI_WriteCMD(CCI_CMD_AGC_SET_HEQ_DAMPENING_FACTOR, 1, &tx);
}

/* --------------------------------------------------------------------------------------------------------------------
 *
 * AGC HEQ Clip Limit High
 *
 * This parameter defines the maximum number of pixels allowed to accumulate in any given histogram bin.
 * Any additional pixels in a given bin are clipped. The effect of this parameter is to limit the influence
 * of highly-populated bins on the resulting HEQ transformation function.
 *
 * --------------------------------------------------------------------------------------------------------------------
 */

#define CCI_CMD_AGC_GET_HEQ_LIMIT_HIGH 0x012C
#define CCI_CMD_AGC_SET_HEQ_LIMIT_HIGH 0x012D

cci_result_t CCI_GetAgcHeqClipLimitHigh(uint16_t* agc_heq_limit_high)
{
	cci_result_t status = CCI_OK;
	cci_union_data_t rx;

	status = CCI_ReadCMD(CCI_CMD_AGC_GET_HEQ_LIMIT_HIGH, 1, &rx);

	if (CCI_OK != status)
		return status;

	*agc_heq_limit_high = rx.uint16;

	return CCI_OK;
}

cci_result_t CCI_SetAgcHeqClipLimitHigh(uint16_t agc_heq_limit_high)
{
	cci_union_data_t tx;

	tx.uint16 = agc_heq_limit_high;

	return CCI_WriteCMD(CCI_CMD_AGC_SET_HEQ_LIMIT_HIGH, 1, &tx);
}

/* --------------------------------------------------------------------------------------------------------------------
 *
 * AGC HEQ Clip Limit Low
 *
 * This parameter defines an artificial population that is added to every non-empty histogram bin.
 * In other words, if the Clip Limit Low is set to L, a bin with an actual population of X will have an effective
 * population of L + X. y empty bin that is nearby a populated bin will be given an artificial population of L.
 * The effect of higher values is to provide a more linear transfer function; lower values provide a more non-linear
 * (equalized) transfer function. This command is deprecated for Lepton 3 (replaced by Linear Percent).
 *
 * --------------------------------------------------------------------------------------------------------------------
 */

#define CCI_CMD_AGC_GET_HEQ_LIMIT_LOW 0x0130
#define CCI_CMD_AGC_SET_HEQ_LIMIT_LOW 0x0131

cci_result_t CCI_GetAgcHeqClipLimitLow(uint16_t* agc_heq_limit_low)
{
	cci_result_t status = CCI_OK;
	cci_union_data_t rx;

	status = CCI_ReadCMD(CCI_CMD_AGC_GET_HEQ_LIMIT_LOW, 1, &rx);

	if (CCI_OK != status)
		return status;

	*agc_heq_limit_low = rx.uint16;

	return CCI_OK;
}

cci_result_t CCI_SetAgcHeqClipLimitLow(uint16_t agc_heq_limit_low)
{
	cci_union_data_t tx;

	tx.uint16 = agc_heq_limit_low;

	return CCI_WriteCMD(CCI_CMD_AGC_SET_HEQ_LIMIT_LOW, 1, &tx);
}

/* --------------------------------------------------------------------------------------------------------------------
 *
 * AGC HEQ Empty Counts
 *
 * This parameter specifies the maximum number of pixels in a bin that will be interpreted as an empty bin.
 *  Histogram bins with this number of pixels or less will be processed as an empty bin.
 *
 * --------------------------------------------------------------------------------------------------------------------
 */

#define CCI_CMD_AGC_GET_HEQ_EMPTY_COUNTS 0x013C
#define CCI_CMD_AGC_SET_HEQ_EMPTY_COUNTS 0x013D

cci_result_t CCI_GetAgcHeqEmptyCount(uint16_t* agc_heq_empty_counts)
{
	cci_result_t status = CCI_OK;
	cci_union_data_t rx;

	status = CCI_ReadCMD(CCI_CMD_AGC_GET_HEQ_EMPTY_COUNTS, 1, &rx);

	if (CCI_OK != status)
		return status;

	*agc_heq_empty_counts = rx.uint16;

	return CCI_OK;
}

cci_result_t CCI_SetAgcHeqEmptyCount(uint16_t agc_heq_empty_counts)
{
	cci_union_data_t tx;

	tx.uint16 = agc_heq_empty_counts;

	return CCI_WriteCMD(CCI_CMD_AGC_SET_HEQ_LIMIT_LOW, 1, &tx);
}

/* --------------------------------------------------------------------------------------------------------------------
 *
 * AGC HEQ Output Scale Factor
 *
 * This parameter specifies the output format for HEQ as either 8-bits (values range 0 to 255), or 14-bit
 * (values range from 0 to 16383).
 *
 * --------------------------------------------------------------------------------------------------------------------
 */

#define CCI_CMD_AGC_GET_HEQ_OUTPUT_SCALE_FACTOR 0x0144
#define CCI_CMD_AGC_SET_HEQ_OUTPUT_SCALE_FACTOR 0x0145

cci_result_t CCI_GetAgcHeqScaleFactor(cci_agc_heq_scale_factor_e* agc_heq_scale_factor)
{
	cci_result_t status = CCI_OK;
	cci_union_data_t rx[2];

	status = CCI_ReadCMD(CCI_CMD_AGC_GET_HEQ_OUTPUT_SCALE_FACTOR, 2, rx);

	if (CCI_OK != status)
		return status;

	*agc_heq_scale_factor = rx[1].uint16 << 16 | rx[0].uint16;

	return CCI_OK;
}

cci_result_t CCI_SetAgcHeqScaleFactor(cci_agc_heq_scale_factor_e agc_heq_scale_factor)
{
	uint32_t value = agc_heq_scale_factor;
	cci_union_data_t tx[2];

	tx[0].uint16 = value & 0xFFFF;
	tx[1].uint16 = (value >> 16) & 0xFFFF;

	return CCI_WriteCMD(CCI_CMD_AGC_SET_HEQ_OUTPUT_SCALE_FACTOR, 2, tx);

}

/* --------------------------------------------------------------------------------------------------------------------
 *
 * AGC Calculation Enable State
 *
 * This parameter controls the camera AGC calculations operations. If enabled, the current video histogram and AGC
 * policy will be calculated for each input frame. If disabled, then no AGC calculations are performed and the current
 * state of the ITT is preserved. For smooth AGC on /off operation, it is recommended to have this enabled.
 *
 * --------------------------------------------------------------------------------------------------------------------
 */

#define CCI_CMD_AGC_GET_CALCULATION_ENABLE_STATE 0x0148
#define CCI_CMD_AGC_SET_CALCULATION_ENABLE_STATE 0x0149

cci_result_t CCI_GetAgcCalcEnableState(cci_agc_enable_e* agc_calc_enable_state)
{
	cci_result_t status = CCI_OK;
	cci_union_data_t rx[2];

	status = CCI_ReadCMD(CCI_CMD_AGC_GET_CALCULATION_ENABLE_STATE, 2, rx);

	if (CCI_OK != status)
		return status;

	*agc_calc_enable_state = rx[1].uint16 << 16 | rx[0].uint16;

	return CCI_OK;
}

cci_result_t CCI_SetAgcCalcEnableState(cci_agc_enable_e agc_calc_enable_state)
{
	uint32_t value = agc_calc_enable_state;
	cci_union_data_t tx[2];

	tx[0].uint16 = value & 0xFFFF;
	tx[1].uint16 = (value >> 16) & 0xFFFF;

	return CCI_WriteCMD(CCI_CMD_AGC_SET_CALCULATION_ENABLE_STATE, 2, tx);

}

/* --------------------------------------------------------------------------------------------------------------------
 *
 * AGC HEQ Linear Percent
 *
 * This parameter controls the camera AGC HEQ algorithm’s linear percent. The linear percent parameter fills holes
 * in the histogram with pixels to avoid undesirable compression in 8-bit irradiance levels.
 * Similar to the low clip limit, pixels are added to each full bin and to an additional number of empty bins
 * following full bins. The linear percent parameter is more automatic than allowing the user to specify the low clip
 * limit; the linear percent adjusts low clip limit based on the scene content and the desired percentage of
 * total pixels in the histogram.
 *
 * --------------------------------------------------------------------------------------------------------------------
 */

#define CCI_CMD_AGC_GET_HEQ_LINEAR_PERCENT 0x014C
#define CCI_CMD_AGC_SET_HEQ_LINEAR_PERCENT 0x014D

cci_result_t CCI_GetAgcHeqLinearPercent(uint16_t* agc_heq_linear_percent)
{
	cci_result_t status = CCI_OK;
	cci_union_data_t rx;

	status = CCI_ReadCMD(CCI_CMD_AGC_GET_HEQ_LINEAR_PERCENT, 1, &rx);

	if (CCI_OK != status)
		return status;

	*agc_heq_linear_percent = rx.uint16;

	return CCI_OK;
}

cci_result_t CCI_SetAgcHeqLinearPercent(uint16_t agc_heq_linear_percent)
{
	cci_union_data_t tx;

	tx.uint16 = agc_heq_linear_percent;

	return CCI_WriteCMD(CCI_CMD_AGC_SET_HEQ_LINEAR_PERCENT, 1, &tx);
}
