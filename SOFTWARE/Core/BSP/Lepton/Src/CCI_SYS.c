/*
 * CCI_SYS.c
 *
 *  Created on: Feb 15, 2024
 *      Author: grzegorz
 */

#include "CCI_SYS.h"


/* --------------------------------------------------------------------------------------------------------------------
 *
 * SYS Status
 *
 * This command returns the system status: System Ready, System Initializing, System in Low-Power Mode,
 * System Going into Standby, and FFC in Progress.
 *
 * --------------------------------------------------------------------------------------------------------------------
 */

#define CCI_CMD_SYS_GET_STATUS				0x0204

cci_result_t CCI_GetSysStatus(cci_sys_status_t* sys_status)
{
	cci_result_t status = CCI_OK;
	cci_union_data_t rx[4];

	status = CCI_ReadCMD(CCI_CMD_SYS_GET_STATUS, 4, rx);

	if (CCI_OK != status)
		return status;

	sys_status->camStatus = rx[1].uint16 << 16 | rx[0].uint16;
	sys_status->commandCount = rx[2].uint16;
	sys_status->reserved = rx[3].uint16;

	return CCI_OK;
}

/* --------------------------------------------------------------------------------------------------------------------
 *
 * SYS FLIR Serial Number
 *
 * This command returns the Lepton Camera’s serial number as a 64-bit unsigned long integer.
 *
 * --------------------------------------------------------------------------------------------------------------------
 */

#define CCI_CMD_SYS_GET_FLIR_SERIAL_NUMBER		0x0208

cci_result_t CCI_SysFlirSerialNumber(uint64_t* flir_serial_number)
{
	cci_result_t status = CCI_OK;
	cci_union_data_t rx[4];

	status = CCI_ReadCMD(CCI_CMD_SYS_GET_FLIR_SERIAL_NUMBER, 4, rx);

	if (CCI_OK != status)
		return status;


	*flir_serial_number = (uint64_t) (((uint64_t)rx[3].uint16 << 48)
			| ((uint64_t)rx[2].uint16 << 32) | ((uint64_t)rx[1].uint16 << 16)
			| (uint64_t)rx[0].uint16);

	return CCI_WaitBusyClear();
}

/* --------------------------------------------------------------------------------------------------------------------
 *
 * SYS Camera Uptime
 *
 * This command returns the Lepton Camera’s current uptime in milliseconds. The uptime is the time since the
 * camera was brought out of Standby. The uptime counter is implemented as a 32-bit counter and as such will roll-
 * over after the maximum count of 0xFFFFFFFF (1193 hours) is reached and restart at 0x00000000.
 *
 * --------------------------------------------------------------------------------------------------------------------
 */

#define CCI_CMD_SYS_GET_UPTIME					0x020C

cci_result_t CCI_GetSysCameraUpTime(uint32_t *up_time)
{
	cci_result_t status = CCI_OK;
	cci_union_data_t rx[2];

	status = CCI_ReadCMD(CCI_CMD_SYS_GET_UPTIME, 2, rx);

	if (CCI_OK != status)
		return status;

	*up_time = (uint32_t)(rx[1].uint16 << 16 | rx[0].uint16);

	return CCI_WaitBusyClear();
}

/* --------------------------------------------------------------------------------------------------------------------
 *
 * SYS AUX Temperature Kelvin
 *
 * This command returns the Lepton Camera’s AUX Temperature in Kelvin.
 *
 * --------------------------------------------------------------------------------------------------------------------
 */

#define CCI_CMD_SYS_GET_AUX_TEMP				0x0210

cci_result_t CCI_GetSysAuxTemperatureKelvin(uint16_t *temperature)
{
	cci_result_t status = CCI_OK;
	cci_union_data_t rx;

	status = CCI_ReadCMD(CCI_CMD_SYS_GET_AUX_TEMP, 1, &rx);

	if (CCI_OK != status)
		return status;

	*temperature = rx.uint16;

	return CCI_WaitBusyClear();
}

/* --------------------------------------------------------------------------------------------------------------------
 *
 * SYS FPA Temperature Kelvin
 *
 * This command returns the Lepton Camera’s FPA Temperature in Kelvin.
 *
 * --------------------------------------------------------------------------------------------------------------------
 */

#define CCI_CMD_SYS_GET_FPA_TEMP				0x0214

cci_result_t CCI_GetSysFpaTemperatureKelvin(uint16_t *temperature)
{
	cci_result_t status = CCI_OK;
	cci_union_data_t rx;

	status = CCI_ReadCMD(CCI_CMD_SYS_GET_FPA_TEMP, 1, &rx);

	if (CCI_OK != status)
		return status;

	*temperature = rx.uint16;

	return CCI_WaitBusyClear();
}

/* --------------------------------------------------------------------------------------------------------------------
 *
 * SYS Telemetry Enable State
 *
 * This command returns the Telemetry Enabled State as an Enum.
 *
 * --------------------------------------------------------------------------------------------------------------------
 */

#define CCI_CMD_SYS_GET_TELEMETRY_ENABLE_STATE	0x0218
#define CCI_CMD_SYS_SET_TELEMETRY_ENABLE_STATE	0x0219

cci_result_t CCI_GetSysTelemetryEnableState(cci_sys_telemetry_enable_state_e* telemetry_enable_state)
{
	cci_result_t status = CCI_OK;
	cci_union_data_t rx[2];

	status = CCI_ReadCMD(CCI_CMD_SYS_GET_FPA_TEMP, 2, rx);

	if (CCI_OK != status)
		return status;

	*telemetry_enable_state = (cci_sys_telemetry_enable_state_e)((uint32_t)rx[1].uint16 << 16 | (uint32_t)rx[0].uint16);

	return CCI_WaitBusyClear();
}

cci_result_t CCI_SetSysTelemetryEnableState(cci_sys_telemetry_enable_state_e telemetry_enable_state)
{
	uint32_t value = telemetry_enable_state;
	cci_union_data_t tx[2];

	tx[0].uint16 = value & 0xFFFF;
	tx[1].uint16 = (value >> 16) & 0xFFFF;

	return CCI_WriteCMD(CCI_CMD_SYS_SET_TELEMETRY_ENABLE_STATE, 2, tx);
}


/* --------------------------------------------------------------------------------------------------------------------
 *
 * SYS Frame Average
 *
 * This command executes the average frames command. Executing this command causes the camera to sum together a number
 * of frames, divide the summed frame by the number of frames summed and generate a result frame containing the average
 * of the summed frames.
 *
 * --------------------------------------------------------------------------------------------------------------------
 */

#define CCI_CMD_SYS_FRAME_AVERAGE	0x0222


cci_result_t CCI_RunFrameAverage(void)
{
	CCI_WriteRegister(CCI_REG_COMMAND, CCI_CMD_SYS_FRAME_AVERAGE);

	return CCI_WaitBusyClear();
}


/* --------------------------------------------------------------------------------------------------------------------
 *
 * SYS Number of Frames to Average
 *
 * This command Gets or Sets the number of frames to average when executing either a frame Average command
 * or a Flat-Field Correction (FFC).
 *
 * --------------------------------------------------------------------------------------------------------------------
 */

#define CCI_CMD_SYS_GET_FARAMES_TO_AVERAGE	0x0224
#define CCI_CMD_SYS_SET_FARAMES_TO_AVERAGE	0x0225

cci_result_t CCI_GetFramesToAverage(cci_sys_frame_average_divisor_e* frame_average_divisor)
{
	cci_result_t status = CCI_OK;
	cci_union_data_t rx[2];

	status = CCI_ReadCMD(CCI_CMD_SYS_GET_FARAMES_TO_AVERAGE, 2, rx);

	if (CCI_OK != status)
		return status;

	*frame_average_divisor = rx[1].uint16 << 16 | rx[0].uint16;

	return CCI_OK;
}

cci_result_t CCI_SetFramesToAverage(cci_sys_frame_average_divisor_e frame_average_divisor)
{
	uint32_t value = frame_average_divisor;
	cci_union_data_t tx[2];

	tx[0].uint16 = value & 0xFFFF;
	tx[1].uint16 = (value >> 16) & 0xFFFF;

	return CCI_WriteCMD(CCI_CMD_SYS_SET_FARAMES_TO_AVERAGE, 2, tx);

}

/* --------------------------------------------------------------------------------------------------------------------
 *
 * SYS Camera Video Scene Statistics
 *
 * This command returns the current scene statistics for the video frame defined by the SYS ROI.
 * The statistics captured are scene mean intensity in counts, minimum and maximum intensity in counts, and the number
 * of pixels in the ROI. Lepton scene intensities range from 0 to 16383. The range drops to 0 to 255 when in 8-bit AGC
 * mode. Maximum number of pixels in the scene depends upon which camera, see below tables. When TLinear mode is enabled
 * (available in the Radiometric releases), the camera output represents temperature values, and the scene statistics
 * are reported in Kelvin x 100.
 *
 * --------------------------------------------------------------------------------------------------------------------
 */

#define CCI_CMD_SYS_GET_SCENE_STATISTICS	0x022C

cci_result_t CCI_GetSysSceneStatistics(cci_sys_scene_statistics_t* scene_statistics)
{
	cci_result_t status = CCI_OK;
	cci_union_data_t rx[4];

	status = CCI_ReadCMD(CCI_CMD_SYS_GET_FARAMES_TO_AVERAGE, 2, rx);

	if (CCI_OK != status)
		return status;

	scene_statistics->meanIntensity = rx[0].uint16;
	scene_statistics->maxIntensity = rx[1].uint16;
	scene_statistics->minIntensity = rx[2].uint16;
	scene_statistics->numPixels = rx[3].uint16;

	return CCI_OK;
}

/* --------------------------------------------------------------------------------------------------------------------
 *
 * SYS Scene ROI Select
 *
 * The camera supports processing of pixels contained within a specified rectangular window or Region of Interest (ROI)
 * to calculate scene statistics. This region is defined by 4 parameters: start column, start row, end column,
 * and end row. The region is adjustable to a sub-window. Maximum extents must exclude a 1-pixel boundary from any edge.
 *
 * --------------------------------------------------------------------------------------------------------------------
 */

#define CCI_CMD_SYS_GET_SCENE_ROI_SELECT	0x0230
#define CCI_CMD_SYS_SET_SCENE_ROI_SELECT	0x0231


cci_result_t CCI_GetSysSceneRoi(cci_sys_video_roi_t* video_roi)
{
	cci_result_t status = CCI_OK;
	cci_union_data_t rx[4];

	status = CCI_ReadCMD(CCI_CMD_SYS_GET_SCENE_ROI_SELECT, 4, rx);

	if (CCI_OK != status)
		return status;

	video_roi->startCol = rx[0].uint16;
	video_roi->startRow = rx[1].uint16;
	video_roi->endCol = rx[2].uint16;
	video_roi->endRow = rx[3].uint16;

	return CCI_OK;
}

cci_result_t CCI_SetSysSceneRoi(cci_sys_video_roi_t video_roi)
{

	cci_union_data_t tx[4];

	tx[0].uint16 = video_roi.startCol;
	tx[1].uint16 = video_roi.startRow;
	tx[2].uint16 = video_roi.endCol;
	tx[3].uint16 = video_roi.endRow;

	return CCI_WriteCMD(CCI_CMD_SYS_SET_SCENE_ROI_SELECT, 2, tx);

}

/* --------------------------------------------------------------------------------------------------------------------
 *
 * SYS Thermal Shutdown Count
 *
 * This command returns the current number of frames remaining before a thermal shutdown is executed once the camera
 * temperature exceeds a high-temperature threshold (around 80 degrees C). Once the camera detects the camera exceeded
 * the thermal threshold, this counter begins to count down until zero. When the count reaches ZERO, the camera will
 * shut itself down. A host can use this value to determine when the camera shuts down due to thermal conditions.
 * The default value of 270 is just over 10 seconds at 26 Hz video.
 *
 * --------------------------------------------------------------------------------------------------------------------
 */

#define CCI_CMD_SYS_GET_THERMAL_SHUTDOWN_COUNT	0x0234

cci_result_t CCI_GetSysThermalShutdownCount(uint16_t* thermal_counts)
{
	cci_result_t status = CCI_OK;
	cci_union_data_t rx;

	status = CCI_ReadCMD(CCI_CMD_SYS_GET_THERMAL_SHUTDOWN_COUNT, 1, &rx);

	if (CCI_OK != status)
		return status;

	*thermal_counts = rx.uint16;

	return CCI_OK;
}

/* --------------------------------------------------------------------------------------------------------------------
 *
 * SYS Shutter Position Control
 *
 * This command is used to manually control the position of the attached shutter if one exists. If there is an attached
 * shutter, then this command will return its current position. If there is no shutter attached,
 * it will return LEP_SYS_SHUTTER_POSITION_UNKNOWN.
 *
 * --------------------------------------------------------------------------------------------------------------------
 */

#define CCI_CMD_SYS_GET_SHUTTER_POSITION	0x0238
#define CCI_CMD_SYS_SET_SHUTTER_POSITION	0x0239

cci_result_t CCI_GetShutterPosition(cci_sys_shutter_position_e* shutter_position)
{
	cci_result_t status = CCI_OK;
	cci_union_data_t rx[2];

	status = CCI_ReadCMD(CCI_CMD_SYS_GET_SHUTTER_POSITION, 2, rx);

	if (CCI_OK != status)
		return status;

	*shutter_position = rx[1].uint16 << 16 | rx[0].uint16;

	return CCI_OK;
}

cci_result_t CCI_SetShutterPosition(cci_sys_shutter_position_e shutter_position)
{
	uint32_t value = shutter_position;
	cci_union_data_t tx[2];

	tx[0].uint16 = value & 0xFFFF;
	tx[1].uint16 = (value >> 16) & 0xFFFF;

	return CCI_WriteCMD(CCI_CMD_SYS_SET_SHUTTER_POSITION, 2, tx);

}

/* --------------------------------------------------------------------------------------------------------------------
 *
 * SYS FFC Mode Control
 *
 * This command controls the FFC mode and shutter control during an FFC. FFC modes allow for manual control, automatic
 * control based upon time or temperature changes, and external control. If a shutter is attached this command controls
 * the shutter activity profile.
 *
 * --------------------------------------------------------------------------------------------------------------------
 */

#define CCI_CMD_SYS_GET_FFC_MODE_CONTROL	0x023C
#define CCI_CMD_SYS_SET_FFC_MODE_CONTROL	0x023D

// TBD

/* --------------------------------------------------------------------------------------------------------------------
 *
 * SYS Run FFC Normalization
 *
 * This command executes the camera’s Flat-Field Correction (FFC) normalization. This command executes
 * synchronously. Internally this command polls the camera status to determine when this command completes,
 * and only returns when completed.
 *
 * --------------------------------------------------------------------------------------------------------------------
 */

#define CCI_CMD_SYS_RUN_FFC						0x0242

cci_result_t CCI_RunSysFFCNormalization(void)
{
	CCI_WriteRegister(CCI_REG_COMMAND, CCI_CMD_SYS_RUN_FFC);

	return CCI_WaitBusyClear();
}



/* --------------------------------------------------------------------------------------------------------------------
 *
 * SYS Gain Mode
 *
 * This command sets the gain state of the camera. High gain mode provides higher responsivity and lower noise
 * metrics for normal operation (default). Low gain mode provides lower responsivity and higher noise metrics, but
 * with the benefit of increased intra-scene range necessary to view hotter scenes. Auto gain mode allows the
 * camera to automatically switch the gain mode based on the temperature of the scene and thresholds configurable
 * by the user in the “SYS Gain Mode Object” command. Auto gain mode can only be enabled when radiometry mode
 * is enabled due to the temperature measurement dependency.
 *
 * --------------------------------------------------------------------------------------------------------------------
 */

#define CCI_CMD_SYS_GET_GAIN_MODE				0x0248
#define CCI_CMD_SYS_SET_GAIN_MODE				0x0249

cci_result_t CCI_GetSysGainMode(cci_sys_gain_mode_e* gain_mode)
{
	cci_result_t status = CCI_OK;
	cci_union_data_t rx[2];

	status = CCI_ReadCMD(CCI_CMD_SYS_GET_GAIN_MODE, 2, rx);

	if (CCI_OK != status)
		return status;

	*gain_mode = (cci_sys_gain_mode_e)((uint32_t)rx[1].uint16 << 16 | (uint32_t)rx[0].uint16);

	return CCI_WaitBusyClear();
}

cci_result_t CCI_SetSysGainMode(cci_sys_gain_mode_e gain_mode)
{
	uint32_t value = (uint32_t)gain_mode;
	cci_union_data_t tx[2];

	tx[0].uint16 = value & 0xFFFF;
	tx[1].uint16 = (value >> 16) & 0xFFFF;

	return CCI_WriteCMD(CCI_CMD_SYS_SET_GAIN_MODE, 2, tx);
}

/* --------------------------------------------------------------------------------------------------------------------
 *
 * SYS FFC States
 *
 * This command gets the current FFC state in the camera.
 *
 * --------------------------------------------------------------------------------------------------------------------
 */

#define CCI_CMD_SYS_GET_FFC_STATES				0x024C

cci_result_t CCI_GetSysFFCStates(cci_sys_ffc_states_e* ffc_state)
{
	cci_result_t status = CCI_OK;
	cci_union_data_t rx[2];

	status = CCI_ReadCMD(CCI_CMD_SYS_GET_GAIN_MODE, 2, rx);

	if (CCI_OK != status)
		return status;

	*ffc_state = (cci_sys_ffc_states_e)((uint32_t)rx[1].uint16 << 16 | (uint32_t)rx[0].uint16);

	return CCI_WaitBusyClear();
}
