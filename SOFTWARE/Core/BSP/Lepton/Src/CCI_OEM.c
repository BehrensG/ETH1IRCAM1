/*
 * CCI_OEM.c
 *
 *  Created on: Feb 20, 2024
 *      Author: BehrensG
 */

#include <string.h>

#include "CCI_OEM.h"

/* --------------------------------------------------------------------------------------------------------------------
 *
 * OEM FLIR Systems Part Number
 *
 * This function returns FLIR Systems’ Camera Part Number. The Camera Part Number is a 32-byte string identifier unique
 * to a specific configuration of the Camera module.
 *
 * --------------------------------------------------------------------------------------------------------------------
 */

#define CCI_CMD_OEM_GET_FLIR_PART_NUMBER 			0x481C

cci_result_t CCI_GetOemFlirPartNumber(char *part_number_string)
{
	cci_result_t status = CCI_OK;
	cci_union_data_t rx[16];
	static char part_number[32];

	status = CCI_ReadCMD(CCI_CMD_OEM_GET_FLIR_PART_NUMBER, 16, rx);

	if (CCI_OK != status)
		return status;

	for (uint8_t x = 0; x < 16; x++)
	{
		part_number[2 * x] = rx[x].uint8[0];
		part_number[2 * x + 1] = rx[x].uint8[1];

	}

	part_number[31] = 0;
	strncpy(part_number_string, part_number, 32);

	return CCI_WaitBusyClear();
}

/* --------------------------------------------------------------------------------------------------------------------
 *
 * OEM Camera Software Revision
 *
 * This function returns the Camera’s software revision for both software processors in the Camera.
 * The Camera’s Software revision is composed of 3 fields: a major version, minor version, and a build number for
 * each processor. Each of the 3 fields is 8-bits.
 *
 * --------------------------------------------------------------------------------------------------------------------
 */

#define CCI_CMD_OEM_GET_CAMERA_SOFTWARE_REVISION 	0x4820

cci_result_t CCI_GetOemSoftwareVersion(cci_oem_sw_version_t *sw_version)
{
	cci_result_t status = CCI_OK;
	cci_union_data_t rx[4];

	status = CCI_ReadCMD(CCI_CMD_OEM_GET_CAMERA_SOFTWARE_REVISION, 4, rx);

	if (CCI_OK != status)
		return status;

	sw_version->dsp_build = rx[0].uint8[1];
	sw_version->dsp_major = rx[0].uint8[0];
	sw_version->dsp_minor = rx[1].uint8[1];

	sw_version->gpp_build = rx[1].uint8[0];
	sw_version->gpp_major = rx[2].uint8[1];
	sw_version->gpp_minor = rx[2].uint8[0];

	sw_version->reserved = rx[3].uint16;

	return CCI_WaitBusyClear();
}

/* --------------------------------------------------------------------------------------------------------------------
 *
 * OEM Video Output Enable
 *
 * This function enables or disables the video output independent of output channel.
 *
 * --------------------------------------------------------------------------------------------------------------------
 */

#define CCI_CMD_OEM_GET_VIDEO_OUTPUT_ENABLE	 		0x4824
#define CCI_CMD_OEM_SET_VIDEO_OUTPUT_ENABLE	 		0x4825

cci_result_t CCI_GetOemVideoOutputEnable(
		cci_oem_video_output_enable_e* video_output_enable)
{
	cci_result_t status = CCI_OK;
	cci_union_data_t rx[2];

	status = CCI_ReadCMD(CCI_CMD_OEM_GET_VIDEO_OUTPUT_ENABLE, 2, rx);

	if (CCI_OK != status)
		return status;

	*video_output_enable = (cci_oem_video_output_enable_e)((uint32_t)rx[1].uint16 << 16 | (uint32_t)rx[0].uint16);

	return CCI_WaitBusyClear();
}

cci_result_t CCI_SetOemVideoOutputEnable(
		cci_oem_video_output_enable_e video_output_enable)
{
	uint32_t value = (uint32_t)video_output_enable;
	cci_union_data_t tx[2];

	tx[0].uint16 = value & 0xFFFF;
	tx[1].uint16 = (value >> 16) & 0xFFFF;

	return CCI_WriteCMD(CCI_CMD_OEM_SET_VIDEO_OUTPUT_ENABLE, 2, tx);
}

/* --------------------------------------------------------------------------------------------------------------------
 *
 * OEM Video Output Format Select
 *
 * This function provides the method to specify or retrieve the current video output format.
 *
 * --------------------------------------------------------------------------------------------------------------------
 */

#define CCI_CMD_OEM_GET_VIDEO_OUTPUT_FORMAT	 		0x4828
#define CCI_CMD_OEM_SET_VIDEO_OUTPUT_FORMAT	 		0x4829

cci_result_t CCI_GetOemVideoOutputFormat(
		cci_oem_video_output_format_e* video_output_format)
{
	cci_result_t status = CCI_OK;
	cci_union_data_t rx[2];

	status = CCI_ReadCMD(CCI_CMD_OEM_GET_VIDEO_OUTPUT_FORMAT, 2, rx);

	if (CCI_OK != status)
		return status;

	*video_output_format = (cci_oem_video_output_format_e)((uint32_t)(rx[1].uint16 << 16) | (uint32_t)rx[0].uint16);

	return CCI_WaitBusyClear();
}

cci_result_t CCI_SetOemVideoOutputFormat(
		cci_oem_video_output_format_e video_output_format)
{
	uint32_t value = (uint32_t)video_output_format;
	cci_union_data_t tx[2];

	tx[0].uint16 = value & 0xFFFF;
	tx[1].uint16 = (value >> 16) & 0xFFFF;

	return CCI_WriteCMD(CCI_CMD_OEM_SET_VIDEO_OUTPUT_FORMAT, 2, tx);
}

/* --------------------------------------------------------------------------------------------------------------------
 *
 * OEM Video Output Source Select
 *
 * This function specifies or retrieves the video output source. The output source allows selecting between processed
 *  video data, unprocessed video data, and a variety of ramp patterns.
 *
 * --------------------------------------------------------------------------------------------------------------------
 */

#define CCI_CMD_OEM_GET_VIDEO_OUTPUT_SOURCE_SELECT	0x482C
#define CCI_CMD_OEM_SET_VIDEO_OUTPUT_SOURCE_SELECT	0x482D

cci_result_t CCI_GetOemVideoOutputSource(
		cci_oem_video_output_source_e *video_output_source)
{
	cci_result_t status = CCI_OK;
	cci_union_data_t rx[2];

	status = CCI_ReadCMD(CCI_CMD_OEM_GET_VIDEO_OUTPUT_SOURCE_SELECT, 2, rx);

	if (CCI_OK != status)
		return status;

	*video_output_source = (cci_oem_video_output_source_e)((uint32_t)(rx[1].uint16
			<< 16) | (uint32_t)rx[0].uint16);

	return CCI_WaitBusyClear();
}

cci_result_t CCI_SetOemVideoOutputSource(
		cci_oem_video_output_source_e video_output_source)
{
	uint32_t value = (uint32_t)video_output_source;
	cci_union_data_t tx[2];

	tx[0].uint16 = value & 0xFFFF;
	tx[1].uint16 = (value >> 16) & 0xFFFF;

	return CCI_WriteCMD(CCI_CMD_OEM_SET_VIDEO_OUTPUT_SOURCE_SELECT, 2, tx);
}

/* --------------------------------------------------------------------------------------------------------------------
 *
 * OEM Run Camera Re-Boot
 *
 * This function commands the Camera to re-boot. The Camera is first shutdown, and then restarts automatically.
 *
 * --------------------------------------------------------------------------------------------------------------------
 */

#define CCI_CMD_OEM_RUN_CAMERA_REBOOT	0x4842

cci_result_t CCI_RunOemReboot(void)
{
	CCI_WriteRegister(CCI_REG_COMMAND, CCI_CMD_OEM_RUN_CAMERA_REBOOT);

	return CCI_WaitBusyClear();
}

/* --------------------------------------------------------------------------------------------------------------------
 *
 * OEM Status
 *
 * This function obtains the current status of an OEM run operation. This function is used whenever an OEM command
 * is issued that executes an operation like the run FFC. Typically, the host polls the status to determine when the
 * command has completed. If the return value is negative, then the operation completed with an error. Positive values
 * indicate an in-process state. Zero indicates the operation completed without error.
 *
 * --------------------------------------------------------------------------------------------------------------------
 */

#define CCI_CMD_OEM_GET_STATUS	0x4848

cci_result_t CCI_GetOemCalStatus(cci_oem_status_e *eom_status)
{
	cci_result_t status = CCI_OK;
	cci_union_data_t rx[2];

	status = CCI_ReadCMD(CCI_CMD_OEM_GET_STATUS, 2, rx);

	if (CCI_OK != status)
		return status;

	*eom_status = (cci_oem_status_e)((uint32_t)(rx[1].uint16 << 16)
			| (uint32_t)rx[0].uint16);

	return CCI_WaitBusyClear();
}

/* --------------------------------------------------------------------------------------------------------------------
 *
 * OEM GPIO Mode Select
 *
 * This function gets and sets the GPIO pins mode. Only get is supported in the current release.
 *
 * --------------------------------------------------------------------------------------------------------------------
 */

#define CCI_CMD_OEM_GET_GPIO_MODE	0x4854
#define CCI_CMD_OEM_SET_GPIO_MODE	0x4855

cci_result_t CCI_GetOemGpioMode(cci_oem_gpio_mode_e* gpio_mode)
{
	cci_result_t status = CCI_OK;
	cci_union_data_t rx[2];

	status = CCI_ReadCMD(CCI_CMD_OEM_GET_GPIO_MODE, 2, rx);

	if (CCI_OK != status)
		return status;

	*gpio_mode = (cci_oem_gpio_mode_e)((uint32_t)(rx[1].uint16 << 16)
			| (uint32_t)rx[0].uint16);

	return CCI_WaitBusyClear();
}

cci_result_t CCI_SetOemGpioMode(cci_oem_gpio_mode_e gpio_mode)
{
	uint32_t value = (uint32_t)gpio_mode;
	cci_union_data_t tx[2];

	tx[0].uint16 = value & 0xFFFF;
	tx[1].uint16 = (value >> 16) & 0xFFFF;

	return CCI_WriteCMD(CCI_CMD_OEM_SET_GPIO_MODE, 2, tx);
}

/* --------------------------------------------------------------------------------------------------------------------
 *
 * OEM Video Output Source Constant Value
 *
 * This function gets and sets the video output source constant value. This value is used when the output source
 * selection is CCI_VIDEO_OUTPUT_SOURCE_CONSTANT. The output video source bypasses all video pipeline
 * processing and directly affects the output video. This command is typically used to set the camera output to a
 * known constant value for downstream system tests and calibration.
 *
 * --------------------------------------------------------------------------------------------------------------------
 */

#define CCI_CMD_OEM_GET_OUTPUT_SOURCE_CONSTANT_VALUE	0x483C
#define CCI_CMD_OEM_SET_OUTPUT_SOURCE_CONSTANT_VALUE	0x483D

cci_result_t CCI_GetOemVideoOutputSourceConstantValue(uint16_t *output_source_constant)
{
	cci_result_t status = CCI_OK;
	cci_union_data_t rx;

	status = CCI_ReadCMD(CCI_CMD_OEM_GET_OUTPUT_SOURCE_CONSTANT_VALUE, 1, &rx);

	if (CCI_OK != status)
		return status;

	*output_source_constant = rx.uint16;

	return CCI_WaitBusyClear();
}

cci_result_t CCI_SetOemVideoOutputSourceConstantValue(uint16_t output_source_constant)
{
	cci_union_data_t tx;

	tx.uint16 = output_source_constant;

	return CCI_WriteCMD(CCI_CMD_OEM_SET_OUTPUT_SOURCE_CONSTANT_VALUE, 1, &tx);
}

/* --------------------------------------------------------------------------------------------------------------------
 *
 * OEM FFC Normalization Target
 *
 * The first two of these commands Get and Set the Flat-Field Correction (FFC) Normalization Target used by the third
 * command to execute a Flat-Field Correction (FFC). The target value is factory set and should not be changed under
 * normal circumstances. The Run command executes an FFC using currently active values for the FFC normalization
 * target and number of frames to average. This command executes synchronously. Poll the OEM Status
 * to determine when this command completes.
 *
 * --------------------------------------------------------------------------------------------------------------------
 */

#define CCI_CMD_OEM_GET_FFC_NORMALIZATION_TARGET	0x4844
#define CCI_CMD_OEM_SET_FFC_NORMALIZATION_TARGET	0x4845
#define CCI_CMD_OEM_RUN_FFC_NORMALIZATION_TARGET	0x4846

cci_result_t CCI_GetOemFFCNormalizationTarget(uint16_t *ffc_normalization_target)
{
	cci_result_t status = CCI_OK;
	cci_union_data_t rx;

	status = CCI_ReadCMD(CCI_CMD_OEM_GET_FFC_NORMALIZATION_TARGET, 1, &rx);

	if (CCI_OK != status)
		return status;

	*ffc_normalization_target = rx.uint16;

	return CCI_WaitBusyClear();
}

cci_result_t CCI_SetOemFFCNormalizationTarget(uint16_t ffc_normalization_target)
{
	cci_union_data_t tx;

	tx.uint16 = ffc_normalization_target;

	return CCI_WriteCMD(CCI_CMD_OEM_SET_FFC_NORMALIZATION_TARGET, 1, &tx);
}

cci_result_t CCI_RunOemFFC(void)
{
	CCI_WriteRegister(CCI_REG_COMMAND, CCI_CMD_OEM_RUN_FFC_NORMALIZATION_TARGET);

	return CCI_WaitBusyClear();
}

/* --------------------------------------------------------------------------------------------------------------------
 *
 * OEM Frame Mean Intensity
 *
 * This function obtains the current frame mean intensity value within the video Region of Interest defined by SYS
 * ROI. Note that this ROI (and the resulting mean) is not the same as that used by AGC Histogram Statistics.
 *
 * --------------------------------------------------------------------------------------------------------------------
 */

#define CCI_CMD_OEM_GET_FRAME_MEAN_INTENSITY	0x4844

cci_result_t CCI_GetOemFrameMean(uint16_t *frame_mean)
{
	cci_result_t status = CCI_OK;
	cci_union_data_t rx;

	status = CCI_ReadCMD(CCI_CMD_OEM_GET_FRAME_MEAN_INTENSITY, 1, &rx);

	if (CCI_OK != status)
		return status;

	*frame_mean = rx.uint16;

	return CCI_WaitBusyClear();
}

/* --------------------------------------------------------------------------------------------------------------------
 *
 * OEM Shutter Profile
 *
 * This function gets and sets the shutter profile.
 *
 * --------------------------------------------------------------------------------------------------------------------
 */

#define CCI_CMD_OEM_GET_SHUTTER_PROFILE		0x4864
#define CCI_CMD_OEM_SET_SHUTTER_PROFILE		0x4865

cci_result_t CCI_GetOemShutterProfileObj(cci_oem_shutter_profile_obj_t* shutter_profile_obj)
{
	cci_result_t status = CCI_OK;
	cci_union_data_t rx[2];

	status = CCI_ReadCMD(CCI_CMD_OEM_GET_SHUTTER_PROFILE, 2, rx);

	if (CCI_OK != status)
		return status;

	shutter_profile_obj->closePeriodInFrames = rx[0].uint16;
	shutter_profile_obj->openPeriodInFrames = rx[1].uint16;

	return CCI_WaitBusyClear();
}

cci_result_t CCI_SetOemShutterProfileObj(cci_oem_shutter_profile_obj_t shutter_profile_obj)
{
	cci_union_data_t tx[2];

	tx[0].uint16 = shutter_profile_obj.closePeriodInFrames;
	tx[1].uint16 = shutter_profile_obj.openPeriodInFrames;

	return CCI_WriteCMD(CCI_CMD_OEM_SET_SHUTTER_PROFILE, 2, tx);
}

/* --------------------------------------------------------------------------------------------------------------------
 *
 * OEM Thermal Shutdown Enable
 *
 * This function enables or disables the camera thermal shutdown intended to protect the camera from heating
 * beyond operational temperature range.
 *
 * --------------------------------------------------------------------------------------------------------------------
 */

#define CCI_CMD_OEM_GET_THERMAL_SHUTTER_PROFILE		0x4868
#define CCI_CMD_OEM_SET_THERMAL_SHUTTER_PROFILE		0x4869

cci_result_t CCI_GetOemThermalShutdownEnable(cci_oem_state_e* state)
{
	cci_result_t status = CCI_OK;
	cci_union_data_t rx[2];

	status = CCI_ReadCMD(CCI_CMD_OEM_GET_THERMAL_SHUTTER_PROFILE, 2, rx);

	if (CCI_OK != status)
		return status;

	*state = (cci_oem_state_e)((int32_t)(rx[1].uint16 << 16)
			| (int32_t)rx[0].uint16);

	return CCI_WaitBusyClear();
}

cci_result_t CCI_SetOemThermalShutdownEnable(cci_oem_state_e state)
{
	uint32_t value = (uint32_t)state;
	cci_union_data_t tx[2];

	tx[0].uint16 = value & 0xFFFF;
	tx[1].uint16 = (value >> 16) & 0xFFFF;

	return CCI_WriteCMD(CCI_CMD_OEM_SET_THERMAL_SHUTTER_PROFILE, 2, tx);
}

/* --------------------------------------------------------------------------------------------------------------------
 *
 * OEM Bad Pixel Replacement Control
 *
 * This function enables or disables the camera’s bad pixel replacement control.
 *
 * --------------------------------------------------------------------------------------------------------------------
 */

#define CCI_CMD_OEM_GET_BAD_PIXEL_REPLACEMENT_CONTROL		0x486C
#define CCI_CMD_OEM_SET_BAD_PIXEL_REPLACEMENT_CONTROL		0x486D

cci_result_t CCI_GetOemBadPixelReplaceControl(cci_oem_state_e* state)
{
	cci_result_t status = CCI_OK;
	cci_union_data_t rx[2];

	status = CCI_ReadCMD(CCI_CMD_OEM_GET_BAD_PIXEL_REPLACEMENT_CONTROL, 2, rx);

	if (CCI_OK != status)
		return status;

	*state = (cci_oem_state_e)((int32_t)(rx[1].uint16 << 16)
			| (int32_t)rx[0].uint16);

	return CCI_WaitBusyClear();
}

cci_result_t CCI_SetOemBadPixelReplaceControl(cci_oem_state_e state)
{
	uint32_t value = (uint32_t)state;
	cci_union_data_t tx[2];

	tx[0].uint16 = value & 0xFFFF;
	tx[1].uint16 = (value >> 16) & 0xFFFF;

	return CCI_WriteCMD(CCI_CMD_OEM_SET_BAD_PIXEL_REPLACEMENT_CONTROL, 2, tx);
}

/* --------------------------------------------------------------------------------------------------------------------
 *
 * OEM Temporal Filter Control
 *
 * This function enables or disables the camera’s temporal filter.
 *
 * --------------------------------------------------------------------------------------------------------------------
 */

#define CCI_CMD_OEM_GET_TEMPORAL_FILTER_CONTROL		0x4870
#define CCI_CMD_OEM_SET_TEMPORAL_FILTER_CONTROL		0x4871

cci_result_t CCI_GetOemTemporalFilterControl(cci_oem_state_e* state)
{
	cci_result_t status = CCI_OK;
	cci_union_data_t rx[2];

	status = CCI_ReadCMD(CCI_CMD_OEM_GET_TEMPORAL_FILTER_CONTROL, 2, rx);

	if (CCI_OK != status)
		return status;

	*state = (cci_oem_state_e)((int32_t)(rx[1].uint16 << 16)
			| (int32_t)rx[0].uint16);

	return CCI_WaitBusyClear();
}

cci_result_t CCI_SetOemTemporalFilterControl(cci_oem_state_e state)
{
	uint32_t value = (uint32_t)state;
	cci_union_data_t tx[2];

	tx[0].uint16 = value & 0xFFFF;
	tx[1].uint16 = (value >> 16) & 0xFFFF;

	return CCI_WriteCMD(CCI_CMD_OEM_SET_TEMPORAL_FILTER_CONTROL, 2, tx);
}

/* --------------------------------------------------------------------------------------------------------------------
 *
 * OEM Column Noise Filter (SCNR) Control
 *
 * This function enables or disables the camera’s column noise filter.
 *
 * --------------------------------------------------------------------------------------------------------------------
 */

#define CCI_CMD_OEM_GET_COLUMN_NOISE_FILTER_CONTROL		0x4874
#define CCI_CMD_OEM_SET_COLUMN_NOISE_FILTER_CONTROL		0x4875

cci_result_t CCI_GetOemColumnNoiseEstimateControl(cci_oem_state_e* state)
{
	cci_result_t status = CCI_OK;
	cci_union_data_t rx[2];

	status = CCI_ReadCMD(CCI_CMD_OEM_GET_COLUMN_NOISE_FILTER_CONTROL, 2, rx);

	if (CCI_OK != status)
		return status;

	*state = (cci_oem_state_e)((int32_t)(rx[1].uint16 << 16)
			| (int32_t)rx[0].uint16);

	return CCI_WaitBusyClear();
}

cci_result_t CCI_SetOemColumnNoiseEstimateControl(cci_oem_state_e state)
{
	uint32_t value = (uint32_t)state;
	cci_union_data_t tx[2];

	tx[0].uint16 = value & 0xFFFF;
	tx[1].uint16 = (value >> 16) & 0xFFFF;

	return CCI_WriteCMD(CCI_CMD_OEM_SET_COLUMN_NOISE_FILTER_CONTROL, 2, tx);
}

/* --------------------------------------------------------------------------------------------------------------------
 *
 * OEM Pixel Noise Filter (SPNR) Control
 *
 * This function enables or disables the camera’s pixel noise filter.
 *
 * --------------------------------------------------------------------------------------------------------------------
 */

#define CCI_CMD_OEM_GET_PIXEL_NOISE_FILTER_CONTROL		0x4878
#define CCI_CMD_OEM_SET_PIXEL_NOISE_FILTER_CONTROL		0x4879

cci_result_t CCI_GetOemPixelNoiseSettings(cci_oem_state_e* state)
{
	cci_result_t status = CCI_OK;
	cci_union_data_t rx[2];

	status = CCI_ReadCMD(CCI_CMD_OEM_GET_PIXEL_NOISE_FILTER_CONTROL, 2, rx);

	if (CCI_OK != status)
		return status;

	*state = (cci_oem_state_e)((int32_t)(rx[1].uint16 << 16)
			| (int32_t)rx[0].uint16);

	return CCI_WaitBusyClear();
}

cci_result_t CCI_SetOemPixelNoiseSettings(cci_oem_state_e state)
{
	uint32_t value = (uint32_t)state;
	cci_union_data_t tx[2];

	tx[0].uint16 = value & 0xFFFF;
	tx[1].uint16 = (value >> 16) & 0xFFFF;

	return CCI_WriteCMD(CCI_CMD_OEM_SET_PIXEL_NOISE_FILTER_CONTROL, 2, tx);
}
