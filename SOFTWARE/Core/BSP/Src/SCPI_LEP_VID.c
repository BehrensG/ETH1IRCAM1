/*
 * SCPI_LEP_VID.c
 *
 *  Created on: Feb 27, 2024
 *      Author: BehrensG
 */


#include "cmsis_os.h"
#include "SCPI_LEP_VID.h"
#include "CCI_VID.h"
#include "Lepton.h"

/* --------------------------------------------------------------------------------------------------------------------
 *
 * VID Pseudo-Color Look-Up Table Select
 *
 * --------------------------------------------------------------------------------------------------------------------
 */

static scpi_choice_def_t vid_pcolor_lut_select[] =
{
    {"WHEEL6", CCI_VID_WHEEL6_LUT},
    {"FUSION", CCI_VID_FUSION_LUT},
    {"RAINBOW", CCI_VID_RAINBOW_LUT},
	{"GLOBOW", CCI_VID_GLOBOW_LUT},
	{"SEPIA", CCI_VID_SEPIA_LUT},
	{"COLOR", CCI_VID_COLOR_LUT},
	{"ICE_FIRE", CCI_VID_ICE_FIRE_LUT},
	{"RAIN", CCI_VID_RAIN_LUT},
	{"USER", CCI_VID_USER_LUT},
    SCPI_CHOICE_LIST_END
};

scpi_result_t SCPI_LeptonVideoPColorLutQ(scpi_t * context)
{
	cci_pcolor_lut_e vid_pcolor_lut;

	if (CCI_OK != CCI_GetVidPcolorLut(&vid_pcolor_lut))
	{
		SCPI_ErrorPush(context, SCPI_ERROR_DEVICE_ERROR);
		return SCPI_RES_ERR;
	}

	SCPI_ResultUInt32(context, (uint32_t)vid_pcolor_lut);

	return SCPI_RES_OK;
}


scpi_result_t SCPI_LeptonVideoPColorLut(scpi_t * context)
{
	int32_t vid_pcolor_lut;
	scpi_result_t result;

	if (!SCPI_ParamChoice(context, vid_pcolor_lut_select, &vid_pcolor_lut, FALSE))
	{
		return SCPI_RES_ERR;
	}

	if (CCI_OK != CCI_SetVidPcolorLut(vid_pcolor_lut))
	{
		SCPI_ErrorPush(context, SCPI_ERROR_DEVICE_ERROR);
		result = SCPI_RES_ERR;
	}

	return result;
}

/* --------------------------------------------------------------------------------------------------------------------
 *
 * VID Focus Calculation Enable State
 *
 * --------------------------------------------------------------------------------------------------------------------
 */

scpi_result_t SCPI_LeptonVideoFocusCalculationEnableStateQ(scpi_t * context)
{
	cci_vid_focus_calc_enable_e focus_calc_enable;

	if (CCI_OK != CCI_GetVidFocusCalcEnableState(&focus_calc_enable))
	{
		SCPI_ErrorPush(context, SCPI_ERROR_DEVICE_ERROR);
		return SCPI_RES_ERR;
	}

	SCPI_ResultUInt32(context, (uint32_t)focus_calc_enable);

	return SCPI_RES_OK;
}

scpi_result_t SCPI_LeptonVideoFocusCalculationEnableState(scpi_t * context)
{
	uint32_t focus_calc_enable;

	if(!SCPI_ParamUInt32(context, &focus_calc_enable, TRUE))
	{
		return SCPI_RES_ERR;
	}

	if(CCI_OK != CCI_SetVidFocusCalcEnableState((cci_vid_focus_calc_enable_e)focus_calc_enable))
	{
		SCPI_ErrorPush(context, SCPI_ERROR_DEVICE_ERROR);
		return SCPI_RES_ERR;
	}

	return SCPI_RES_OK;
}

/* --------------------------------------------------------------------------------------------------------------------
 *
 * VID Focus ROI Select
 *
 * --------------------------------------------------------------------------------------------------------------------
 */

scpi_result_t SCPI_LeptonVideoFocusRoiQ(scpi_t * context)
{
	cci_vid_focus_roi_t focus_roi;
	char text[16];

	memset(text,'\0',16);

	if (CCI_OK != CCI_GetVidFocusROI(&focus_roi))
	{
		SCPI_ErrorPush(context, SCPI_ERROR_DEVICE_ERROR);
		return SCPI_RES_ERR;
	}

    sprintf(text,"%d,%d,%d,%d",
    		focus_roi.startCol,
			focus_roi.startRow,
			focus_roi.endCol,
			focus_roi.endRow);

    SCPI_ResultCharacters(context, text, strlen(text));

	return SCPI_RES_OK;
}

scpi_result_t SCPI_LeptonVideoFocusRoi(scpi_t * context)
{
	uint32_t array[4];
	size_t o_count;
	cci_vid_focus_roi_t focus_roi;

	if(!SCPI_ParamArrayUInt32(context, array, 4, &o_count, SCPI_FORMAT_ASCII, TRUE))
	{
		SCPI_ErrorPush(context, SCPI_ERROR_DEVICE_ERROR);
		return SCPI_RES_ERR;
	}

	if (4 != o_count)
	{
		SCPI_ErrorPush(context, SCPI_ERROR_ILLEGAL_PARAMETER_VALUE);
		return SCPI_RES_ERR;
	}

	focus_roi.startCol = array[0];
	focus_roi.startRow = array[1];
	focus_roi.endCol = array[2];
	focus_roi.endRow = array[3];

	if(CCI_OK != CCI_SetVidFocusROI(focus_roi))
	{
		SCPI_ErrorPush(context, SCPI_ERROR_DEVICE_ERROR);
		return SCPI_RES_ERR;
	}

	return SCPI_RES_OK;
}

/* --------------------------------------------------------------------------------------------------------------------
 *
 * VID Focus Metric Threshold
 *
 * --------------------------------------------------------------------------------------------------------------------
 */

scpi_result_t SCPI_LeptonVideoFocusMetricThresholdQ(scpi_t * context)
{
	uint32_t focus_metric_threshold;

	if (CCI_OK != CCI_GetVidFocusMetricThreshold(&focus_metric_threshold))
	{
		SCPI_ErrorPush(context, SCPI_ERROR_DEVICE_ERROR);
		return SCPI_RES_ERR;
	}

	SCPI_ResultUInt32(context, focus_metric_threshold);

	return SCPI_RES_OK;
}

scpi_result_t SCPI_LeptonVideoFocusMetricThreshold(scpi_t * context)
{
	uint32_t focus_metric_threshold;

	if(!SCPI_ParamUInt32(context, &focus_metric_threshold, TRUE))
	{
		return SCPI_RES_ERR;
	}

	if(CCI_OK != CCI_SetVidFocusMetricThreshold(focus_metric_threshold))
	{
		SCPI_ErrorPush(context, SCPI_ERROR_DEVICE_ERROR);
		return SCPI_RES_ERR;
	}

	return SCPI_RES_OK;
}

/* --------------------------------------------------------------------------------------------------------------------
 *
 * VID Focus Metric
 *
 * --------------------------------------------------------------------------------------------------------------------
 */

scpi_result_t SCPI_LeptonVideoFocusMetricQ(scpi_t * context)
{
	uint32_t focus_metric;

	if (CCI_OK != CCI_GetVidFocusMetric(&focus_metric))
	{
		SCPI_ErrorPush(context, SCPI_ERROR_DEVICE_ERROR);
		return SCPI_RES_ERR;
	}

	SCPI_ResultUInt32(context, focus_metric);

	return SCPI_RES_OK;
}

/* --------------------------------------------------------------------------------------------------------------------
 *
 * VID Video Freeze Enable State
 *
 * --------------------------------------------------------------------------------------------------------------------
 */

scpi_result_t SCPI_LeptonVideoFreezeEnableStateQ(scpi_t * context)
{
	uint32_t freeze_enable_state;

	if (CCI_OK != CCI_GetVidFreezeEnableState(&freeze_enable_state))
	{
		SCPI_ErrorPush(context, SCPI_ERROR_DEVICE_ERROR);
		return SCPI_RES_ERR;
	}

	SCPI_ResultUInt32(context, freeze_enable_state);

	return SCPI_RES_OK;
}

scpi_result_t SCPI_LeptonVideoFreezeEnableState(scpi_t * context)
{
	uint32_t freeze_enable_state;

	if(!SCPI_ParamUInt32(context, &freeze_enable_state, TRUE))
	{
		return SCPI_RES_ERR;
	}

	if(CCI_OK != CCI_SetVidFreezeEnableState(freeze_enable_state))
	{
		SCPI_ErrorPush(context, SCPI_ERROR_DEVICE_ERROR);
		return SCPI_RES_ERR;
	}

	return SCPI_RES_OK;
}


extern lep_data_t lepton;

scpi_result_t SCPI_LeptonVideoTimeoutQ(scpi_t * context)
{
	SCPI_ResultUInt32(context, lepton.udp_client.timeout);
	return SCPI_RES_OK;
}

scpi_result_t SCPI_LeptonVideoTimeout(scpi_t * context)
{
	uint32_t timeout;

	if(!SCPI_ParamUInt32(context, &timeout, TRUE))
	{
		return SCPI_RES_ERR;
	}

	lepton.udp_client.timeout = timeout;

	return SCPI_RES_OK;
}
