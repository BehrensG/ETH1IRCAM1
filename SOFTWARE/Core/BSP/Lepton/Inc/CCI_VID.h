/*
 * CCI_VID.h
 *
 *  Created on: Feb 27, 2024
 *      Author: BehrensG
 */

#ifndef BSP_LEPTON_INC_CCI_VID_H_
#define BSP_LEPTON_INC_CCI_VID_H_

#include "CCI.h"

typedef enum
{
	CCI_VID_WHEEL6_LUT=0,
	CCI_VID_FUSION_LUT,
	CCI_VID_RAINBOW_LUT,
	CCI_VID_GLOBOW_LUT,
	CCI_VID_SEPIA_LUT,
	CCI_VID_COLOR_LUT,
	CCI_VID_ICE_FIRE_LUT,
	CCI_VID_RAIN_LUT,
	CCI_VID_USER_LUT

}cci_pcolor_lut_e;

typedef enum
{
	CCI_VID_FOCUS_CALC_DISABLE=0,
	CCI_VID_FOCUS_CALC_ENABLE

}cci_vid_focus_calc_enable_e;

typedef struct
{
	uint16_t startCol;
	uint16_t startRow;
	uint16_t endCol;
	uint16_t endRow;

}cci_vid_focus_roi_t;

typedef enum
{
	CCI_VID_VIDEO_OUTPUT_FORMAT_RAW8 = 0,
	CCI_VID_VIDEO_OUTPUT_FORMAT_RAW10,
	CCI_VID_VIDEO_OUTPUT_FORMAT_RAW12,
	CCI_VID_VIDEO_OUTPUT_FORMAT_RGB888,
	CCI_VID_VIDEO_OUTPUT_FORMAT_RGB666,
	CCI_VID_VIDEO_OUTPUT_FORMAT_RGB565,
	CCI_VID_VIDEO_OUTPUT_FORMAT_YUV422_8BIT,
	CCI_VID_VIDEO_OUTPUT_FORMAT_RAW14,
	CCI_VID_VIDEO_OUTPUT_FORMAT_YUV422_10BIT,
	CCI_VID_VIDEO_OUTPUT_FORMAT_USER_DEFINED,
	CCI_VID_VIDEO_OUTPUT_FORMAT_RAW8_2,
	CCI_VID_VIDEO_OUTPUT_FORMAT_RAW8_3,
	CCI_VID_VIDEO_OUTPUT_FORMAT_RAW8_4,
	CCI_VID_VIDEO_OUTPUT_FORMAT_RAW8_5,
	CCI_VID_VIDEO_OUTPUT_FORMAT_RAW8_6

}cci_vid_video_output_format_e;

cci_result_t CCI_GetVidPcolorLut(cci_pcolor_lut_e* vid_pcolor_lut);
cci_result_t CCI_SetVidPcolorLut(cci_pcolor_lut_e vid_pcolor_lut);

cci_result_t CCI_GetVidFocusCalcEnableState(cci_vid_focus_calc_enable_e* focus_calc_enable);
cci_result_t CCI_SetVidFocusCalcEnableState(cci_vid_focus_calc_enable_e focus_calc_enable);

cci_result_t CCI_GetVidFocusROI(cci_vid_focus_roi_t* focus_roi);
cci_result_t CCI_SetVidFocusROI(cci_vid_focus_roi_t focus_roi);

cci_result_t CCI_GetVidFocusMetricThreshold(uint32_t* focus_metric_threshold);
cci_result_t CCI_SetVidFocusMetricThreshold(uint32_t focus_metric_threshold);

cci_result_t CCI_GetVidFocusMetric(uint32_t* focus_metric);

cci_result_t CCI_GetVidFreezeEnableState(uint32_t* freeze_enable_state);
cci_result_t CCI_SetVidFreezeEnableState(uint32_t freeze_enable_state);

cci_result_t CCI_GetVidVideoOutputFormat(cci_vid_video_output_format_e* video_output_format);
cci_result_t CCI_SetVidVideoOutputFormat(cci_vid_video_output_format_e video_output_format);

cci_result_t CCI_GetVidLowGainPcolorLut(cci_pcolor_lut_e* pcolor_lut);
cci_result_t CCI_SetVidLowGainPcolorLut(cci_pcolor_lut_e pcolor_lut);

#endif /* BSP_LEPTON_INC_CCI_VID_H_ */
