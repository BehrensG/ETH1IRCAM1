/*
 * SCPI_LEP_OEM.c
 *
 *  Created on: Feb 22, 2024
 *      Author: grzegorz
 */

#include <string.h>

#include "cmsis_os.h"

#include "SCPI_LEP_OEM.h"

#include "CCI_OEM.h"
#include "CCI_AGC.h"
#include "CCI_SYS.h"
#include "CCI_RAD.h"
#include "CCI_VID.h"

#include "Lepton.h"

extern lep_data_t lepton;
extern scpi_choice_def_t scpi_boolean_select[];

/* --------------------------------------------------------------------------------------------------------------------
 *
 * OEM FLIR Systems Part Number
 *
 * --------------------------------------------------------------------------------------------------------------------
 */

scpi_result_t SCPI_LeptonOemPartNumberQ(scpi_t *context)
{
	SCPI_ResultText(context, lepton.info.oem_part_number);

	return SCPI_RES_OK;
}

/* --------------------------------------------------------------------------------------------------------------------
 *
 * OEM Camera Software Revision
 *
 * --------------------------------------------------------------------------------------------------------------------
 */

scpi_result_t SCPI_LeptonOemSoftwareRevisionQ(scpi_t *context)
{

	char buff[18];
	buff[17]='\0';

	cci_oem_sw_version_t sw_version;

	if (CCI_OK != CCI_GetOemSoftwareVersion(&sw_version))
	{
		SCPI_ErrorPush(context, SCPI_ERROR_DEVICE_ERROR);
		return SCPI_RES_ERR;
	}

	sprintf(buff, "%02x.%02x.%02x,%02x.%02x.%02x", sw_version.dsp_major,
									 sw_version.dsp_minor,
									 sw_version.dsp_build,
									 sw_version.gpp_major,
									 sw_version.gpp_minor,
									 sw_version.gpp_build);

	SCPI_ResultText(context, buff);
	return SCPI_RES_OK;
}

/* --------------------------------------------------------------------------------------------------------------------
 *
 * OEM Video Output Enable
 *
 * This function enables or disables the video output independent of output channel.
 *
 * --------------------------------------------------------------------------------------------------------------------
 */


scpi_result_t SCPI_LeptonOemVideoOutputEnableQ(scpi_t *context)
{

	cci_oem_video_output_enable_e video_output_enable;

	if (CCI_OK != CCI_GetOemVideoOutputEnable(&video_output_enable))
	{
		SCPI_ErrorPush(context, SCPI_ERROR_DEVICE_ERROR);
		return SCPI_RES_ERR;
	}

	SCPI_ResultUInt8(context, (uint8_t)video_output_enable);

	return SCPI_RES_OK;
}

scpi_result_t SCPI_LeptonOemVideoOutputEnable(scpi_t *context)
{
	int32_t video_output_enable;

	if (!SCPI_ParamChoice(context, scpi_boolean_select, &video_output_enable, TRUE))
	{
		return SCPI_RES_ERR;
	}

	if (CCI_OK != CCI_SetOemVideoOutputEnable((cci_oem_video_output_enable_e)video_output_enable))
	{
		SCPI_ErrorPush(context, SCPI_ERROR_DEVICE_ERROR);
		return SCPI_RES_ERR;
	}

	return SCPI_RES_OK;
}

/* --------------------------------------------------------------------------------------------------------------------
 *
 * OEM Video Output Format Select
 *
 * --------------------------------------------------------------------------------------------------------------------
 */

static scpi_choice_def_t video_output_format_select[] =
{
    {"RGB888", CCI_VIDEO_OUTPUT_FORMAT_RGB888},
    {"RAW14", CCI_VIDEO_OUTPUT_FORMAT_RAW14},
    SCPI_CHOICE_LIST_END
};

scpi_result_t SCPI_LeptonOemVideoOutputFormatQ(scpi_t * context)
{

	cci_oem_video_output_format_e video_output_format;

	if (CCI_OK != CCI_GetOemVideoOutputFormat(&video_output_format))
	{
		SCPI_ErrorPush(context, SCPI_ERROR_DEVICE_ERROR);
		return SCPI_RES_ERR;
	}

	SCPI_ResultUInt16(context, (uint16_t)video_output_format);

	return SCPI_RES_OK;
}

static scpi_result_t SCPI_LeptonEnableRAW14(cci_oem_video_output_format_e video_format_select)
{
	if(CCI_OK != CCI_SetAgcEnableState(CCI_AGC_DISABLE))
		return SCPI_RES_ERR;

	if(CCI_OK != CCI_SetRadTLinearEnableState(CCI_RAD_ENABLE))
		return SCPI_RES_ERR;

	if (CCI_OK != CCI_SetOemVideoOutputFormat(video_format_select))
		return SCPI_RES_ERR;

	return SCPI_RES_OK;
}

static scpi_result_t SCPI_LeptonEnableRGB888(cci_oem_video_output_format_e video_format_select)
{
	if(CCI_OK != CCI_SetRadTLinearEnableState(CCI_RAD_DISABLE))
		return SCPI_RES_ERR;

	if(CCI_OK != CCI_SetAgcEnableState(CCI_AGC_ENABLE))
		return SCPI_RES_ERR;

	if (CCI_OK != CCI_SetOemVideoOutputFormat(video_format_select))
		return SCPI_RES_ERR;

	if (CCI_OK != CCI_SetVidPcolorLut(CCI_VID_FUSION_LUT))
		return SCPI_RES_ERR;

	return SCPI_RES_OK;
}

scpi_result_t SCPI_LeptonOemVideoOutputFormat(scpi_t * context)
{
	int32_t video_format_select;
	scpi_result_t result;

	if (!SCPI_ParamChoice(context, video_output_format_select, &video_format_select, TRUE))
	{
		return SCPI_RES_ERR;
	}


	if(CCI_VIDEO_OUTPUT_FORMAT_RAW14 == video_format_select)
	{
		result = SCPI_LeptonEnableRAW14(video_format_select);
	}
	else
	{
		result = SCPI_LeptonEnableRGB888(video_format_select);
	}

	if(SCPI_RES_OK == result)
		lepton.config.image_format = video_format_select;

	return result;
}

/* --------------------------------------------------------------------------------------------------------------------
 *
 * OEM Video Output Source Select
 *
 * --------------------------------------------------------------------------------------------------------------------
 */

static scpi_choice_def_t video_output_source_select[] =
{
    {"RAW", CCI_VIDEO_OUTPUT_SOURCE_RAW},
    {"COOKED", CCI_VIDEO_OUTPUT_SOURCE_COOKED},
    {"RAMP", CCI_VIDEO_OUTPUT_SOURCE_RAMP},
	{"CONSTANT", CCI_VIDEO_OUTPUT_SOURCE_CONSTANT},
	{"RAMP_H", CCI_VIDEO_OUTPUT_SOURCE_RAMP_H},
	{"RAMP_V", CCI_VIDEO_OUTPUT_SOURCE_RAMP_V},
	{"CUSTOM", CCI_VIDEO_OUTPUT_SOURCE_RAMP_CUSTOM},
	{"CAPTURE", CCI_VIDEO_OUTPUT_SOURCE_FRAME_CAPTURE},
	{"FREEZE", CCI_VIDEO_OUTPUT_SOURCE_FRAME_FREEZE},
    SCPI_CHOICE_LIST_END
};


scpi_result_t SCPI_LeptonOemVideoOutputSourceQ(scpi_t * context)
{
	cci_oem_video_output_source_e video_output_source;

	if (CCI_OK != CCI_GetOemVideoOutputSource(&video_output_source))
	{
		SCPI_ErrorPush(context, SCPI_ERROR_DEVICE_ERROR);
		return SCPI_RES_ERR;
	}

	SCPI_ResultUInt16(context, (uint16_t)video_output_source);

	return SCPI_RES_OK;
}

scpi_result_t SCPI_LeptonOemVideoOutputSource(scpi_t * context)
{
	int32_t video_output_source;

	if (!SCPI_ParamChoice(context, video_output_source_select, &video_output_source, TRUE))
	{
		return SCPI_RES_ERR;
	}

	if (CCI_OK != CCI_SetOemVideoOutputSource((cci_oem_video_output_source_e)video_output_source))
	{
		SCPI_ErrorPush(context, SCPI_ERROR_DEVICE_ERROR);
		return SCPI_RES_ERR;
	}

	return SCPI_RES_OK;
}

/* --------------------------------------------------------------------------------------------------------------------
 *
 * OEM Run Camera Re-Boot
 *
 * * --------------------------------------------------------------------------------------------------------------------
 */

extern osThreadId_t LeptonMainTaskHandle;
extern osThreadId_t LeptonUDPTaskHandle;

scpi_result_t SCPI_LeptonOemReboot(scpi_t * context)
{

	if (CCI_OK != CCI_RunOemReboot())
	{
		SCPI_ErrorPush(context, SCPI_ERROR_DEVICE_ERROR);
		return SCPI_RES_ERR;
	}

	if (osOK != osThreadTerminate(LeptonMainTaskHandle))
	{
		SCPI_ErrorPush(context, SCPI_ERROR_SYSTEM_ERROR);
		return SCPI_RES_ERR;
	}

	if (osOK != osThreadTerminate(LeptonUDPTaskHandle))
	{
		SCPI_ErrorPush(context, SCPI_ERROR_SYSTEM_ERROR);
		return SCPI_RES_ERR;
	}

	LEP_CreateMainTask();
	return SCPI_RES_OK;
}

/* --------------------------------------------------------------------------------------------------------------------
 *
 * OEM Status
 *
 * --------------------------------------------------------------------------------------------------------------------
 */

scpi_result_t SCPI_LeptonOemStatus(scpi_t * context)
{
	cci_oem_status_e oem_status;

	if (CCI_OK != CCI_GetOemCalStatus(&oem_status))
	{
		SCPI_ErrorPush(context, SCPI_ERROR_DEVICE_ERROR);
		return SCPI_RES_ERR;
	}

	SCPI_ResultInt32(context, (int32_t)oem_status);

	return SCPI_RES_OK;
}

/* --------------------------------------------------------------------------------------------------------------------
 *
 * OEM Video Output Source Constant Value
 *
 * --------------------------------------------------------------------------------------------------------------------
 */

scpi_result_t SCPI_LeptonOemVideoOutputSourceConstantValueQ(scpi_t * context)
{
	uint16_t output_source_constant;

	if (CCI_OK != CCI_GetOemVideoOutputSourceConstantValue(&output_source_constant))
	{
		SCPI_ErrorPush(context, SCPI_ERROR_DEVICE_ERROR);
		return SCPI_RES_ERR;
	}

	SCPI_ResultUInt16(context, output_source_constant);

	return SCPI_RES_OK;
}

scpi_result_t SCPI_LeptonOemVideoOutputSourceConstantValue(scpi_t * context)
{
	uint32_t output_source_constant;

	if (!SCPI_ParamUInt32(context, &output_source_constant, TRUE))
		return SCPI_RES_ERR;

	if (CCI_OK != CCI_SetOemVideoOutputSourceConstantValue((uint16_t)output_source_constant))
	{
		SCPI_ErrorPush(context, SCPI_ERROR_DEVICE_ERROR);
		return SCPI_RES_ERR;
	}

	return SCPI_RES_OK;
}

/* --------------------------------------------------------------------------------------------------------------------
 *
 * OEM FFC Normalization Target
 *
 * --------------------------------------------------------------------------------------------------------------------
 */

scpi_result_t SCPI_LeptonOemFFCNormalizationTargetQ(scpi_t * context)
{
	uint16_t ffc_normalization_target;

	if (CCI_OK != CCI_GetOemFFCNormalizationTarget(&ffc_normalization_target))
	{
		SCPI_ErrorPush(context, SCPI_ERROR_DEVICE_ERROR);
		return SCPI_RES_ERR;
	}

	SCPI_ResultUInt16(context, ffc_normalization_target);

	return SCPI_RES_OK;
}

scpi_result_t SCPI_LeptonOemFFCNormalizationTarget(scpi_t * context)
{
	uint32_t ffc_normalization_target;

	if (!SCPI_ParamUInt32(context, &ffc_normalization_target, TRUE))
		return SCPI_RES_ERR;

	if (CCI_OK != CCI_SetOemVideoOutputSourceConstantValue((uint16_t)ffc_normalization_target))
	{
		SCPI_ErrorPush(context, SCPI_ERROR_DEVICE_ERROR);
		return SCPI_RES_ERR;
	}

	return SCPI_RES_OK;
}

scpi_result_t SCPI_LeptonRunOemFFC(scpi_t * context)
{

	if (CCI_OK != CCI_RunOemFFC())
	{
		SCPI_ErrorPush(context, SCPI_ERROR_DEVICE_ERROR);
		return SCPI_RES_ERR;
	}

	return SCPI_RES_OK;
}

/* --------------------------------------------------------------------------------------------------------------------
 *
 * OEM Frame Mean Intensity
 *
 * --------------------------------------------------------------------------------------------------------------------
 */

scpi_result_t SCPI_LeptonOemFrameMeanIntensityQ(scpi_t * context)
{
	uint16_t frame_mean_intensity;

	if (CCI_OK != CCI_GetOemFrameMean(&frame_mean_intensity))
	{
		SCPI_ErrorPush(context, SCPI_ERROR_DEVICE_ERROR);
		return SCPI_RES_ERR;
	}

	SCPI_ResultUInt16(context, frame_mean_intensity);

	return SCPI_RES_OK;
}

/* --------------------------------------------------------------------------------------------------------------------
 *
 * OEM Shutter Profile
 *
 * --------------------------------------------------------------------------------------------------------------------
 */

scpi_result_t SCPI_LeptonOemShutterProfileQ(scpi_t * context)
{
	// TBD
	return SCPI_RES_OK;
}
scpi_result_t SCPI_LeptonOemShutterProfile(scpi_t * context)
{
	// TBD
	return SCPI_RES_OK;
}

/* --------------------------------------------------------------------------------------------------------------------
 *
 * OEM Thermal Shutdown Enable
 *
 * --------------------------------------------------------------------------------------------------------------------
 */

scpi_result_t SCPI_LeptonOemThermalShutdownEnableQ(scpi_t * context)
{
	cci_oem_state_e thermal_shutdown_state;

	if (CCI_OK != CCI_GetOemThermalShutdownEnable(&thermal_shutdown_state))
	{
		SCPI_ErrorPush(context, SCPI_ERROR_DEVICE_ERROR);
		return SCPI_RES_ERR;
	}

	SCPI_ResultUInt32(context, (uint32_t)thermal_shutdown_state);

	return SCPI_RES_OK;
}

scpi_result_t SCPI_LeptonOemThermalShutdownEnable(scpi_t * context)
{
	int32_t thermal_shutdown_state;

	if (!SCPI_ParamChoice(context, scpi_boolean_select, &thermal_shutdown_state, TRUE))
	{
		return SCPI_RES_ERR;
	}

	if (CCI_OK != CCI_SetOemThermalShutdownEnable((cci_oem_state_e)thermal_shutdown_state))
	{
		SCPI_ErrorPush(context, SCPI_ERROR_DEVICE_ERROR);
		return SCPI_RES_ERR;
	}

	return SCPI_RES_OK;
}

/* --------------------------------------------------------------------------------------------------------------------
 *
 * OEM Bad Pixel Replacement Control
 *
 * --------------------------------------------------------------------------------------------------------------------
 */

scpi_result_t SCPI_LeptonOemBadPixelReplacmenetControlQ(scpi_t * context)
{
	cci_oem_state_e bad_pixel_replacement_control;

	if (CCI_OK != CCI_GetOemBadPixelReplaceControl(&bad_pixel_replacement_control))
	{
		SCPI_ErrorPush(context, SCPI_ERROR_DEVICE_ERROR);
		return SCPI_RES_ERR;
	}

	SCPI_ResultUInt32(context, (uint32_t)bad_pixel_replacement_control);

	return SCPI_RES_OK;
}

scpi_result_t SCPI_LeptonOemBadPixelReplacmenetControl(scpi_t * context)
{
	int32_t bad_pixel_replacement_control;

	if (!SCPI_ParamChoice(context, scpi_boolean_select, &bad_pixel_replacement_control, TRUE))
	{
		return SCPI_RES_ERR;
	}

	if (CCI_OK != CCI_SetOemBadPixelReplaceControl((cci_oem_state_e)bad_pixel_replacement_control))
	{
		SCPI_ErrorPush(context, SCPI_ERROR_DEVICE_ERROR);
		return SCPI_RES_ERR;
	}

	return SCPI_RES_OK;
}

/* --------------------------------------------------------------------------------------------------------------------
 *
 * OEM Temporal Filter Control
 *
 * --------------------------------------------------------------------------------------------------------------------
 */

scpi_result_t SCPI_LeptonOemTemporalFilterControlQ(scpi_t * context)
{
	cci_oem_state_e temporal_filter_control;

	if (CCI_OK != CCI_GetOemTemporalFilterControl(&temporal_filter_control))
	{
		SCPI_ErrorPush(context, SCPI_ERROR_DEVICE_ERROR);
		return SCPI_RES_ERR;
	}

	SCPI_ResultUInt32(context, (uint32_t)temporal_filter_control);

	return SCPI_RES_OK;
}

scpi_result_t SCPI_LeptonOemTemporalFilterControl(scpi_t * context)
{
	int32_t temporal_filter_control;

	if (!SCPI_ParamChoice(context, scpi_boolean_select, &temporal_filter_control, TRUE))
	{
		return SCPI_RES_ERR;
	}

	if (CCI_OK != CCI_SetOemTemporalFilterControl((cci_oem_state_e)temporal_filter_control))
	{
		SCPI_ErrorPush(context, SCPI_ERROR_DEVICE_ERROR);
		return SCPI_RES_ERR;
	}

	return SCPI_RES_OK;
}

/* --------------------------------------------------------------------------------------------------------------------
 *
 * OEM Column Noise Filter (SCNR) Control
 *
 * --------------------------------------------------------------------------------------------------------------------
 */

scpi_result_t SCPI_LeptonOemColumnNoiseFilterControlQ(scpi_t * context)
{
	cci_oem_state_e column_noise_filter_control;

	if (CCI_OK != CCI_GetOemColumnNoiseEstimateControl(&column_noise_filter_control))
	{
		SCPI_ErrorPush(context, SCPI_ERROR_DEVICE_ERROR);
		return SCPI_RES_ERR;
	}

	SCPI_ResultUInt32(context, (uint32_t)column_noise_filter_control);

	return SCPI_RES_OK;
}

scpi_result_t SCPI_LeptonOemColumnNoiseFilterControl(scpi_t * context)
{
	int32_t column_noise_filter_control;

	if (!SCPI_ParamChoice(context, scpi_boolean_select, &column_noise_filter_control, TRUE))
	{
		return SCPI_RES_ERR;
	}

	if (CCI_OK != CCI_SetOemColumnNoiseEstimateControl((cci_oem_state_e)column_noise_filter_control))
	{
		SCPI_ErrorPush(context, SCPI_ERROR_DEVICE_ERROR);
		return SCPI_RES_ERR;
	}

	return SCPI_RES_OK;
}

/* --------------------------------------------------------------------------------------------------------------------
 *
 * OEM Pixel Noise Filter (SPNR) Control
 *
 * --------------------------------------------------------------------------------------------------------------------
 */

scpi_result_t SCPI_LeptonOemPixelNoiseFilterControlQ(scpi_t * context)
{
	cci_oem_state_e pixel_noise_filter_control;

	if (CCI_OK != CCI_GetOemPixelNoiseSettings(&pixel_noise_filter_control))
	{
		SCPI_ErrorPush(context, SCPI_ERROR_DEVICE_ERROR);
		return SCPI_RES_ERR;
	}

	SCPI_ResultUInt32(context, (uint32_t)pixel_noise_filter_control);

	return SCPI_RES_OK;
}

scpi_result_t SCPI_LeptonOemPixelNoiseFilterControl(scpi_t * context)
{
	int32_t pixel_noise_filter_control;

	if (!SCPI_ParamChoice(context, scpi_boolean_select, &pixel_noise_filter_control, TRUE))
	{
		return SCPI_RES_ERR;
	}

	if (CCI_OK != CCI_SetOemPixelNoiseSettings((cci_oem_state_e)pixel_noise_filter_control))
	{
		SCPI_ErrorPush(context, SCPI_ERROR_DEVICE_ERROR);
		return SCPI_RES_ERR;
	}

	return SCPI_RES_OK;
}
