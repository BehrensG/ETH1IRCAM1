/*
 * CCI_SYS.h
 *
 *  Created on: Feb 15, 2024
 *      Author: grzegorz
 */

#ifndef BSP_LEPTON_INC_CCI_SYS_H_
#define BSP_LEPTON_INC_CCI_SYS_H_

#include "CCI.h"


typedef enum
{
	CCI_SYSTEM_READY=0,
	CCI_SYSTEM_INITIALIZING,
	CCI_SYSTEM_IN_LOW_POWER_MODE,
	CCI_SYSTEM_GOING_INTO_STANDBY,
	CCI_SYSTEM_FLAT_FIELD_IN_PROCESS

}cci_system_status_states_e;

typedef struct
{
	uint32_t camStatus;
	uint16_t commandCount;
	uint16_t reserved;

}cci_sys_status_t;


typedef enum
{
	CCI_SYS_FA_DIV_1 = 0,
	CCI_SYS_FA_DIV_2,
	CCI_SYS_FA_DIV_4,
	CCI_SYS_FA_DIV_8,
	CCI_SYS_FA_DIV_16,
	CCI_SYS_FA_DIV_32,
	CCI_SYS_FA_DIV_64,
	CCI_SYS_FA_DIV_128

}cci_sys_frame_average_divisor_e;


typedef struct
{
	uint16_t meanIntensity;
	uint16_t maxIntensity;
	uint16_t minIntensity;
	uint16_t numPixels;

}cci_sys_scene_statistics_t;

typedef struct
{
	uint16_t startCol;
	uint16_t startRow;
	uint16_t endCol;
	uint16_t endRow;

}cci_sys_video_roi_t;


typedef enum
{
	CCI_SYS_SHUTTER_POSITION_UNKNOWN = -1,
	CCI_SYS_SHUTTER_POSITION_IDLE = 0,
	CCI_SYS_SHUTTER_POSITION_OPEN,
	CCI_SYS_SHUTTER_POSITION_CLOSED,
	CCI_SYS_SHUTTER_POSITION_BRAKE_ON

}cci_sys_shutter_position_e;


typedef enum CCI_SYS_FFC_SHUTTER_MODE_E_TAG
{
	CCI_SYS_FFC_SHUTTER_MODE_MANUAL = 0,
	CCI_SYS_FFC_SHUTTER_MODE_AUTO,
	CCI_SYS_FFC_SHUTTER_MODE_EXTERNAL

}cci_sys_ffc_shutter_mode_e;

typedef enum CCI_SYS_SHUTTER_TEMP_LOCKOUT_STATE_E_TAG
{
	CCI_SYS_SHUTTER_LOCKOUT_INACTIVE = 0, /* not locked out */
	CCI_SYS_SHUTTER_LOCKOUT_HIGH, /* lockout due to high temp */
	CCI_SYS_SHUTTER_LOCKOUT_LOW /* lockout due to low temp */

}cci_sys_shutter_temp_lockout_state_e;

typedef struct CCI_SYS_FFC_SHUTTER_MODE_OBJ_T_TAG
{
	uint32_t shutterMode; /* defines current mode */
	uint32_t tempLockoutState;
	uint32_t videoFreezeDuringFFC;
	uint32_t ffcDesired; /* status of FFC desired */
	uint32_t elapsedTimeSinceLastFfc; /* in milliseconds x1 */
	uint32_t desiredFfcPeriod; /* in milliseconds x1 */
	bool explicitCmdToOpen; /* true or false */
	uint16_t desiredFfcTempDelta; /* in Kelvin x100 */
	uint16_t imminentDelay; /* in frame counts x1 */

}cci_sys_ffc_shutter_mode_obj_t;

typedef enum
{
	CCI_TELEMETRY_DISABLED = 0,
	CCI_TELEMETRY_ENABLED

}cci_sys_telemetry_enable_state_e;


typedef enum
{
	CCI_SYS_GAIN_MODE_HIGH = 0,
	CCI_SYS_GAIN_MODE_LOW,
	CCI_SYS_GAIN_MODE_AUTO

} cci_sys_gain_mode_e;

typedef enum
{
	CCI_SYS_FFC_NEVER_COMMANDED = 0,
	CCI_SYS_FFC_IMMINENT,
	CCI_SYS_FFC_IN_PROCESS,
	CCI_SYS_FFC_DONE

}cci_sys_ffc_states_e;

cci_result_t CCI_GetSysStatus(cci_sys_status_t* sys_status);

cci_result_t CCI_SysFlirSerialNumber(uint64_t* flir_serial_number);

cci_result_t CCI_GetSysCameraUpTime(uint32_t *up_time);

cci_result_t CCI_GetSysAuxTemperatureKelvin(uint16_t *temperature);

cci_result_t CCI_GetSysFpaTemperatureKelvin(uint16_t *temperature);

cci_result_t CCI_GetSysTelemetryEnableState(cci_sys_telemetry_enable_state_e* telemetry_enable_state);
cci_result_t CCI_SetSysTelemetryEnableState(cci_sys_telemetry_enable_state_e telemetry_enable_state);

cci_result_t CCI_RunFrameAverage(void);

cci_result_t CCI_GetFramesToAverage(cci_sys_frame_average_divisor_e* frame_average_divisor);
cci_result_t CCI_SetFramesToAverage(cci_sys_frame_average_divisor_e frame_average_divisor);

cci_result_t CCI_GetSysSceneStatistics(cci_sys_scene_statistics_t* scene_statistics);

cci_result_t CCI_GetSysSceneRoi(cci_sys_video_roi_t* video_roi);
cci_result_t CCI_SetSysSceneRoi(cci_sys_video_roi_t video_roi);

cci_result_t CCI_GetSysThermalShutdownCount(uint16_t* thermal_counts);

cci_result_t CCI_GetShutterPosition(cci_sys_shutter_position_e* shutter_position);
cci_result_t CCI_SetShutterPosition(cci_sys_shutter_position_e shutter_position);

cci_result_t CCI_RunSysFFCNormalization(void);

cci_result_t CCI_GetSysGainMode(cci_sys_gain_mode_e* gain_mode);
cci_result_t CCI_SetSysGainMode(cci_sys_gain_mode_e gain_mode);

cci_result_t CCI_GetSysFFCStates(cci_sys_ffc_states_e* ffc_state);

#endif /* BSP_LEPTON_INC_CCI_SYS_H_ */
