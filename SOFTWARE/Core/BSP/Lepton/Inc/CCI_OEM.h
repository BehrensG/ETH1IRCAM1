/*
 * CCI_OEM.h
 *
 *  Created on: Feb 20, 2024
 *      Author: BehrensG
 */

#ifndef BSP_LEPTON_INC_CCI_OEM_H_
#define BSP_LEPTON_INC_CCI_OEM_H_

#include "main.h"
#include "CCI.h"

typedef struct
{
	uint8_t gpp_major;
	uint8_t gpp_minor;
	uint8_t gpp_build;
	uint8_t dsp_major;
	uint8_t dsp_minor;
	uint8_t dsp_build;
	uint16_t reserved;

} cci_oem_sw_version_t;

typedef enum
{
	CCI_VIDEO_OUTPUT_DISABLE = 0,
	CCI_VIDEO_OUTPUT_ENABLE

}cci_oem_video_output_enable_e;


typedef enum
{
	CCI_VIDEO_OUTPUT_FORMAT_RGB888 = 3,
	CCI_VIDEO_OUTPUT_FORMAT_RAW14 = 7

}cci_oem_video_output_format_e;


typedef enum
{
	CCI_VIDEO_OUTPUT_SOURCE_RAW = 0, /* Before video processing. */
	CCI_VIDEO_OUTPUT_SOURCE_COOKED, /* Post video processing - NORMAL MODE */
	CCI_VIDEO_OUTPUT_SOURCE_RAMP, /* Software Ramp pattern - increase in X, Y */
	CCI_VIDEO_OUTPUT_SOURCE_CONSTANT, /* Software Constant value pattern */
	CCI_VIDEO_OUTPUT_SOURCE_RAMP_H, /* Software Ramp pattern - increase in X only */
	CCI_VIDEO_OUTPUT_SOURCE_RAMP_V, /* Software Ramp pattern - increase in Y only */
	CCI_VIDEO_OUTPUT_SOURCE_RAMP_CUSTOM, /* Software Ramp pattern - uses custom settings */
	CCI_VIDEO_OUTPUT_SOURCE_FRAME_CAPTURE, // Average, Capture frame
	CCI_VIDEO_OUTPUT_SOURCE_FRAME_FREEZE, // Freeze-Frame Buffer

}cci_oem_video_output_source_e;

typedef enum
{
	CCI_OEM_STATUS_OTP_WRITE_ERROR = -2,
	CCI_OEM_STATUS_ERROR = -1,
	CCI_OEM_STATUS_READY = 0,
	CCI_OEM_STATUS_BUSY,
	CCI_OEM_FRAME_AVERAGE_COLLECTING_FRAMES

}cci_oem_status_e;


typedef enum
{
	CCI_OEM_GPIO_MODE_GPIO = 0,
	CCI_OEM_GPIO_MODE_I2C_MASTER = 1,
	CCI_OEM_GPIO_MODE_SPI_MASTER_VLB_DATA = 2,
	CCI_OEM_GPIO_MODE_SPIO_MASTER_REG_DATA = 3,
	CCI_OEM_GPIO_MODE_SPI_SLAVE_VLB_DATA = 4,
	CCI_OEM_GPIO_MODE_VSYNC = 5

}cci_oem_gpio_mode_e;

typedef struct
{
	uint16_t closePeriodInFrames;
	uint16_t openPeriodInFrames;

}cci_oem_shutter_profile_obj_t;

typedef enum
{
	CCI_OEM_DISABLE = 0,
	CCI_OEM_ENABLE

}cci_oem_state_e;

cci_result_t CCI_GetOemFlirPartNumber(char *part_number_string);

cci_result_t CCI_GetOemSoftwareVersion(cci_oem_sw_version_t *sw_version);

cci_result_t CCI_GetOemVideoOutputFormat(cci_oem_video_output_format_e *video_output_format);
cci_result_t CCI_SetOemVideoOutputFormat(cci_oem_video_output_format_e video_output_format);

cci_result_t CCI_GetOemVideoOutputSource(cci_oem_video_output_source_e *video_output_source);
cci_result_t CCI_SetOemVideoOutputSource(cci_oem_video_output_source_e video_output_source);

cci_result_t CCI_GetOemVideoOutputEnable(cci_oem_video_output_enable_e *video_output_enable);
cci_result_t CCI_SetOemVideoOutputEnable(cci_oem_video_output_enable_e video_output_enable);

cci_result_t CCI_GetOemVideoOutputSourceConstantValue(uint16_t *output_source_constant);
cci_result_t CCI_SetOemVideoOutputSourceConstantValue(uint16_t output_source_constant);

cci_result_t CCI_GetOemFFCNormalizationTarget(uint16_t *ffc_normalization_target);
cci_result_t CCI_SetOemFFCNormalizationTarget(uint16_t ffc_normalization_target);
cci_result_t CCI_RunOemFFC(void);

cci_result_t CCI_GetOemFrameMean(uint16_t *frame_mean);

cci_result_t CCI_GetOemShutterProfileObj(cci_oem_shutter_profile_obj_t* shutter_profile_obj);
cci_result_t CCI_SetOemShutterProfileObj(cci_oem_shutter_profile_obj_t shutter_profile_obj);

cci_result_t CCI_GetOemThermalShutdownEnable(cci_oem_state_e* state);
cci_result_t CCI_SetOemThermalShutdownEnable(cci_oem_state_e state);

cci_result_t CCI_GetOemBadPixelReplaceControl(cci_oem_state_e* state);
cci_result_t CCI_SetOemBadPixelReplaceControl(cci_oem_state_e state);

cci_result_t CCI_GetOemTemporalFilterControl(cci_oem_state_e* state);
cci_result_t CCI_SetOemTemporalFilterControl(cci_oem_state_e state);

cci_result_t CCI_GetOemColumnNoiseEstimateControl(cci_oem_state_e* state);
cci_result_t CCI_SetOemColumnNoiseEstimateControl(cci_oem_state_e state);

cci_result_t CCI_GetOemPixelNoiseSettings(cci_oem_state_e* state);
cci_result_t CCI_SetOemPixelNoiseSettings(cci_oem_state_e state);

cci_result_t CCI_SetOemGpioMode(cci_oem_gpio_mode_e gpio_mode);

cci_result_t CCI_RunOemReboot(void);

cci_result_t CCI_GetOemCalStatus(cci_oem_status_e *eom_status);

#endif /* BSP_LEPTON_INC_CCI_OEM_H_ */
