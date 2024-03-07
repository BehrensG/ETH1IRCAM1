/*
 * SCPI_LeptonSYS.c
 *
 *  Created on: Feb 15, 2024
 *      Author: grzegorz
 */

#include <stdio.h>

#include "SCPI_LEP_SYS.h"
#include "CCI_SYS.h"

extern bsp_t bsp;

/* --------------------------------------------------------------------------------------------------------------------
 *
 * SYS Status
 *
 * --------------------------------------------------------------------------------------------------------------------
 */

scpi_result_t SCPI_LeptonSystemSystemStatusQ (scpi_t * context)
{
	cci_sys_status_t sys_status;

	if (CCI_OK != CCI_GetSysStatus(&sys_status))
	{

		SCPI_ErrorPush(context, SCPI_ERROR_DEVICE_ERROR);
		return SCPI_RES_ERR;
	}

	SCPI_ResultUInt32(context, sys_status.camStatus);

	return SCPI_RES_OK;
}

/* --------------------------------------------------------------------------------------------------------------------
 *
 * SYS FLIR Serial Number
 *
 * --------------------------------------------------------------------------------------------------------------------
 */

scpi_result_t SCPI_LeptonSystemSerialNumberQ(scpi_t * context)
{
	uint64_t serial_number;
	char str[18] = {0};

	if(CCI_OK != CCI_SysFlirSerialNumber(&serial_number))
	{

		SCPI_ErrorPush(context, SCPI_ERROR_DEVICE_ERROR);
		return SCPI_RES_ERR;
	}

	sprintf(str, "%X", serial_number);
	SCPI_ResultMnemonic(context, (char*)str);

	return SCPI_RES_OK;
}

/* --------------------------------------------------------------------------------------------------------------------
 *
 * SYS Camera Uptime
 *
 * --------------------------------------------------------------------------------------------------------------------
 */

scpi_result_t SCPI_LeptonSystemUpTimeQ(scpi_t * context)
{
	uint32_t up_time;

	if (CCI_OK != CCI_GetSysCameraUpTime(&up_time))
	{

		SCPI_ErrorPush(context, SCPI_ERROR_DEVICE_ERROR);
		return SCPI_RES_ERR;
	}

	SCPI_ResultUInt32(context, up_time);

	return SCPI_RES_OK;
}

/* --------------------------------------------------------------------------------------------------------------------
 *
 * SYS AUX Temperature Kelvin
 *
 * --------------------------------------------------------------------------------------------------------------------
 */

scpi_result_t SCPI_LeptonSystemAUXTemperatureQ(scpi_t * context)
{
	float float_temp = 0;
	uint16_t uint16_temp = 0;

	if(CCI_OK != CCI_GetSysAuxTemperatureKelvin(&uint16_temp))
	{

		SCPI_ErrorPush(context, SCPI_ERROR_DEVICE_ERROR);
		return SCPI_RES_ERR;
	}

	switch (bsp.temperature_unit)
	{
		case CELSIUS: float_temp = (float)(uint16_temp/100) - 273.15;  break;
		case FAHRENHEIT: float_temp = ((float)(uint16_temp/100) - 273.15)*1.8+32; break;
		case KELVIN: float_temp = (float)(uint16_temp/100) ; break;
	}

	SCPI_ResultFloat(context, float_temp);

	return SCPI_RES_OK;
}

/* --------------------------------------------------------------------------------------------------------------------
 *
 * SYS FPA Temperature Kelvin
 *
 * --------------------------------------------------------------------------------------------------------------------
 */

scpi_result_t SCPI_LeptonSystemFPATemperatureQ(scpi_t * context)
{
	float float_temp = 0;
	uint16_t uint16_temp = 0;

	if (CCI_OK != CCI_GetSysFpaTemperatureKelvin(&uint16_temp))
	{

		SCPI_ErrorPush(context, SCPI_ERROR_DEVICE_ERROR);
		return SCPI_RES_ERR;
	}

	switch (bsp.temperature_unit)
	{
		case CELSIUS: float_temp = (float)(uint16_temp/100) - 273.15;  break;
		case FAHRENHEIT: float_temp = ((float)(uint16_temp/100) - 273.15)*1.8+32; break;
		case KELVIN: float_temp = (float)(uint16_temp/100) ; break;
	}

	SCPI_ResultFloat(context, float_temp);

	return SCPI_RES_OK;
}

/* --------------------------------------------------------------------------------------------------------------------
 *
 * SYS Telemetry Enable State
 *
 * --------------------------------------------------------------------------------------------------------------------
 */

scpi_result_t SCPI_LeptonSystemTelemetryEnableStateQ(scpi_t * context)
{
	cci_sys_telemetry_enable_state_e telemetry_enable_state;

	if (CCI_OK != CCI_GetSysTelemetryEnableState(&telemetry_enable_state))
	{
		SCPI_ErrorPush(context, SCPI_ERROR_DEVICE_ERROR);
		return SCPI_RES_ERR;
	}

	SCPI_ResultUInt32(context, (uint32_t)telemetry_enable_state);

	return SCPI_RES_OK;
}

scpi_result_t SCPI_LeptonSystemTelemetryEnableState(scpi_t * context)
{
	uint32_t telemetry_enable_state;

	if(!SCPI_ParamUInt32(context, &telemetry_enable_state, TRUE))
	{
		return SCPI_RES_ERR;
	}

	if(CCI_OK != CCI_SetSysTelemetryEnableState((cci_sys_telemetry_enable_state_e)telemetry_enable_state))
	{
		SCPI_ErrorPush(context, SCPI_ERROR_DEVICE_ERROR);
		return SCPI_RES_ERR;
	}

	return SCPI_RES_OK;
}

/* --------------------------------------------------------------------------------------------------------------------
 *
 * SYS Frame Average
 *
 * --------------------------------------------------------------------------------------------------------------------
 */

scpi_result_t SCPI_LeptonSystemFrameAverageRun(scpi_t * context)
{

	if(CCI_OK != CCI_RunFrameAverage())
	{
		SCPI_ErrorPush(context, SCPI_ERROR_DEVICE_ERROR);
		return SCPI_RES_ERR;
	}

	return SCPI_RES_OK;
}

/* --------------------------------------------------------------------------------------------------------------------
 *
 * SYS Number of Frames to Average
 *
 * --------------------------------------------------------------------------------------------------------------------
 */

scpi_result_t SCPI_LeptonSystemFrameToAverageQ(scpi_t * context)
{
	//TBD
	return SCPI_RES_OK;
}

scpi_result_t SCPI_LeptonSystemFrameToAverage(scpi_t * context)
{
	uint32_t frame_average_divisor;

	if(!SCPI_ParamUInt32(context, &frame_average_divisor, TRUE))
	{
		return SCPI_RES_ERR;
	}

	if(CCI_OK != CCI_SetFramesToAverage((cci_sys_frame_average_divisor_e)frame_average_divisor))
	{
		SCPI_ErrorPush(context, SCPI_ERROR_DEVICE_ERROR);
		return SCPI_RES_ERR;
	}

	return SCPI_RES_OK;
}

/* --------------------------------------------------------------------------------------------------------------------
 *
 * SYS Camera Video Scene Statistics
 *
 * --------------------------------------------------------------------------------------------------------------------
 */

scpi_result_t SCPI_LeptonSystemSceneStatisticsQ(scpi_t * context)
{
	cci_sys_scene_statistics_t scene_statistics;
	char text[64];

	memset(text,'\0',64);

	if (CCI_OK != CCI_GetSysSceneStatistics(&scene_statistics))
	{
		SCPI_ErrorPush(context, SCPI_ERROR_DEVICE_ERROR);
		return SCPI_RES_ERR;
	}

    sprintf(text,"%d,%d,%d,%d",
    		scene_statistics.meanIntensity,
			scene_statistics.maxIntensity,
			scene_statistics.minIntensity,
			scene_statistics.numPixels);

    text[63] = '\0';

    SCPI_ResultCharacters(context, text, strlen(text));

    return SCPI_RES_OK;
}

/* --------------------------------------------------------------------------------------------------------------------
 *
 * SYS Scene ROI Select
 *
 * --------------------------------------------------------------------------------------------------------------------
 */

scpi_result_t SCPI_LeptonSystemSceneRoiQ(scpi_t * context)
{
	cci_sys_video_roi_t video_roi;
	char text[16];

	memset(text,'\0',16);

	if (CCI_OK != CCI_GetSysSceneRoi(&video_roi))
	{
		SCPI_ErrorPush(context, SCPI_ERROR_DEVICE_ERROR);
		return SCPI_RES_ERR;
	}


    sprintf(text,"%d,%d,%d,%d",
    		video_roi.startCol,
			video_roi.startRow,
			video_roi.endCol,
			video_roi.endRow);

    text[15] = '\0';

    SCPI_ResultCharacters(context, text, strlen(text));

	return SCPI_RES_OK;
}

scpi_result_t SCPI_LeptonSystemSceneRoi(scpi_t * context)
{
	uint32_t array[4];
	size_t o_count;
	cci_sys_video_roi_t video_roi;

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

	video_roi.startCol = array[0];
	video_roi.startRow = array[1];
	video_roi.endCol = array[2];
	video_roi.endRow = array[3];

	if(CCI_OK != CCI_SetSysSceneRoi(video_roi))
	{
		SCPI_ErrorPush(context, SCPI_ERROR_DEVICE_ERROR);
		return SCPI_RES_ERR;
	}

	return SCPI_RES_OK;
}

/* --------------------------------------------------------------------------------------------------------------------
 *
 * SYS Thermal Shutdown Count
 *
 * --------------------------------------------------------------------------------------------------------------------
 */

scpi_result_t SCPI_LeptonOemThermalShutdownCountQ(scpi_t * context)
{
	uint16_t thermal_counts;

	if (CCI_OK != CCI_GetSysThermalShutdownCount(&thermal_counts))
	{
		SCPI_ErrorPush(context, SCPI_ERROR_DEVICE_ERROR);
		return SCPI_RES_ERR;
	}

	SCPI_ResultUInt16(context, thermal_counts);

	return SCPI_RES_OK;
}

/* --------------------------------------------------------------------------------------------------------------------
 *
 * SYS Shutter Position Control
 *
 * --------------------------------------------------------------------------------------------------------------------
 */

scpi_result_t SCPI_LeptonSystemShutterPositionQ(scpi_t * context)
{
	cci_sys_shutter_position_e shutter_position;

	if (CCI_OK != CCI_GetShutterPosition(&shutter_position))
	{
		SCPI_ErrorPush(context, SCPI_ERROR_DEVICE_ERROR);
		return SCPI_RES_ERR;
	}

	SCPI_ResultUInt32(context, (uint32_t)shutter_position);

	return SCPI_RES_OK;
}

scpi_result_t SCPI_LeptonSystemShutterPosition(scpi_t * context)
{
	uint32_t shutter_position;

	if(!SCPI_ParamUInt32(context, &shutter_position, TRUE))
	{
		return SCPI_RES_ERR;
	}

	if(CCI_OK != CCI_SetShutterPosition((cci_sys_shutter_position_e)shutter_position))
	{
		SCPI_ErrorPush(context, SCPI_ERROR_DEVICE_ERROR);
		return SCPI_RES_ERR;
	}

	return SCPI_RES_OK;
}

/* --------------------------------------------------------------------------------------------------------------------
 *
 * SYS Run FFC Normalization
 *
 * --------------------------------------------------------------------------------------------------------------------
 */

scpi_result_t SCPI_LeptonSystemFFCNormalization(scpi_t * context)
{

	if (CCI_OK != CCI_RunSysFFCNormalization())
	{

		SCPI_ErrorPush(context, SCPI_ERROR_DEVICE_ERROR);
		return SCPI_RES_ERR;
	}

	return SCPI_RES_OK;
}

/* --------------------------------------------------------------------------------------------------------------------
 *
 * SYS Gain Mode
 *
 * --------------------------------------------------------------------------------------------------------------------
 */

scpi_result_t SCPI_LeptonSystemGainModeQ(scpi_t * context)
{
	cci_sys_gain_mode_e gain_mode;

	if (CCI_OK != CCI_GetSysGainMode(&gain_mode))
	{
		SCPI_ErrorPush(context, SCPI_ERROR_DEVICE_ERROR);
		return SCPI_RES_ERR;
	}

	SCPI_ResultUInt32(context, (uint32_t)gain_mode);

	return SCPI_RES_OK;
}


scpi_result_t SCPI_LeptonSystemGainMode(scpi_t * context)
{
	uint32_t gain_mode;

	if(!SCPI_ParamUInt32(context, &gain_mode, TRUE))
	{
		return SCPI_RES_ERR;
	}

	if(CCI_OK != CCI_SetSysGainMode((cci_sys_gain_mode_e)gain_mode))
	{
		SCPI_ErrorPush(context, SCPI_ERROR_DEVICE_ERROR);
		return SCPI_RES_ERR;
	}

	return SCPI_RES_OK;
}

/* --------------------------------------------------------------------------------------------------------------------
 *
 * SYS FFC States
 *
 * --------------------------------------------------------------------------------------------------------------------
 */

scpi_result_t SCPI_LeptonSystemFFCStatusQ(scpi_t * context)
{
	cci_sys_ffc_states_e ffc_state;

	if (CCI_OK != CCI_GetSysFFCStates(&ffc_state))
	{
		SCPI_ErrorPush(context, SCPI_ERROR_DEVICE_ERROR);
		return SCPI_RES_ERR;
	}

	SCPI_ResultUInt32(context, (uint32_t)ffc_state);

	return SCPI_RES_OK;
}
