/*
 * CCI_VID.c
 *
 *  Created on: Feb 27, 2024
 *      Author: BehrensG
 */

#include "CCI_VID.h"

/* --------------------------------------------------------------------------------------------------------------------
 *
 * VID Pseudo-Color Look-Up Table Select
 *
 * This function allows selection of the video output pseudo-color LUT. This LUT applies to the video processed by camera
 * post AGC application before output. Color LUTs do not apply to raw video output of any format.
 * Requires using the video output format of 24-bit R, G, B.
 *
 * --------------------------------------------------------------------------------------------------------------------
 */

#define CCI_CMD_VID_GET_PCOLOR_LUT_STATE	0x0304
#define CCI_CMD_VID_SET_PCOLOR_LUT_STATE	0x0305

cci_result_t CCI_GetVidPcolorLut(cci_pcolor_lut_e* vid_pcolor_lut)
{
	cci_result_t status = CCI_OK;
	cci_union_data_t rx[2];

	status = CCI_ReadCMD(CCI_CMD_VID_GET_PCOLOR_LUT_STATE, 2, rx);

	if (CCI_OK != status)
		return status;

	*vid_pcolor_lut = (cci_pcolor_lut_e) ((uint32_t) (rx[1].uint16
			<< 16) | (uint32_t) rx[0].uint16);

	return CCI_WaitBusyClear();
}

cci_result_t CCI_SetVidPcolorLut(cci_pcolor_lut_e vid_pcolor_lut)
{
	uint32_t value = (uint32_t)vid_pcolor_lut;
	cci_union_data_t tx[2];

	tx[0].uint16 = value & 0xFFFF;
	tx[1].uint16 = (value >> 16) & 0xFFFF;

	return CCI_WriteCMD(CCI_CMD_VID_SET_PCOLOR_LUT_STATE, 2, tx);
}

/* --------------------------------------------------------------------------------------------------------------------
 *
 * VID Focus Calculation Enable State
 *
 * The camera can calculate a video scene focus metric (also useful as a metric of contrast). This function specifies
 * whether or not the camera is to make these calculations on the input video. When enabled, the camera will
 * calculate the video scene focus metric on each frame processed and make the result available in the focus metric.
 * When disabled, the camera does not execute the focus metric calculation.
 * Note that AGC must be disabled when the focus metric is enabled.
 *
 * --------------------------------------------------------------------------------------------------------------------
 */

#define CCI_CMD_VID_GET_FOCUS_CALC_ENABLE_STATE	0x030C
#define CCI_CMD_VID_SET_FOCUS_CALC_ENABLE_STATE	0x030D

cci_result_t CCI_GetVidFocusCalcEnableState(cci_vid_focus_calc_enable_e* focus_calc_enable)
{
	cci_result_t status = CCI_OK;
	cci_union_data_t rx[2];

	status = CCI_ReadCMD(CCI_CMD_VID_GET_FOCUS_CALC_ENABLE_STATE, 2, rx);

	if (CCI_OK != status)
		return status;

	*focus_calc_enable = (cci_vid_focus_calc_enable_e) ((uint32_t) (rx[1].uint16
			<< 16) | (uint32_t) rx[0].uint16);

	return CCI_WaitBusyClear();
}

cci_result_t CCI_SetVidFocusCalcEnableState(cci_vid_focus_calc_enable_e focus_calc_enable)
{
	uint32_t value = (uint32_t)focus_calc_enable;
	cci_union_data_t tx[2];

	tx[0].uint16 = value & 0xFFFF;
	tx[1].uint16 = (value >> 16) & 0xFFFF;

	return CCI_WriteCMD(CCI_CMD_VID_SET_FOCUS_CALC_ENABLE_STATE, 2, tx);
}

/* --------------------------------------------------------------------------------------------------------------------
 *
 * VID Focus ROI Select
 *
 * The camera supports processing of pixels contained within a specified rectangular window or Region of Interest
 * (ROI) to calculate a focus metric. This region is defined by 4 parameters: start column, start row, end column, and
 * end row. The region is adjustable to a sub-window. Maximum extents must exclude a 1-pixel boundary from any edge.
 *
 * --------------------------------------------------------------------------------------------------------------------
 */

#define CCI_CMD_VID_GET_FOCUS_ROI	0x0310
#define CCI_CMD_VID_SET_FOCUS_ROI	0x0311

cci_result_t CCI_GetVidFocusROI(cci_vid_focus_roi_t* focus_roi)
{
	cci_result_t status = CCI_OK;
	cci_union_data_t rx[4];

	status = CCI_ReadCMD(CCI_CMD_VID_GET_FOCUS_ROI, 4, rx);

	if (CCI_OK != status)
		return status;

	focus_roi->startCol = rx[0].uint16;
	focus_roi->startRow = rx[1].uint16;
	focus_roi->endCol = rx[2].uint16;
	focus_roi->endRow = rx[3].uint16;

	return CCI_WaitBusyClear();
}

cci_result_t CCI_SetVidFocusROI(cci_vid_focus_roi_t focus_roi)
{
	cci_union_data_t tx[4];

	tx[0].uint16 = focus_roi.startCol;
	tx[1].uint16 = focus_roi.startRow;
	tx[2].uint16 = focus_roi.endCol;
	tx[3].uint16 = focus_roi.endRow;

	return CCI_WriteCMD(CCI_CMD_VID_SET_FOCUS_ROI, 4, tx);
}

/* --------------------------------------------------------------------------------------------------------------------
 *
 * VID Focus Metric Threshold
 *
 * This function specifies the focus metric threshold. The focus metric evaluates image gradients and counts the
 * number of gradient magnitudes that exceed the focus metric threshold. Therefore, larger values of the threshold
 * imply the focus metric is counting gradients with larger magnitudes in effect filtering out small gradients in the
 * image (pixel noise, for example). The Focus Metric uses the Tenengrad method which is an edge-based metric that
 * measures the sum of the horizontal and vertical gradients using Sobel operators. The Focus Metric Threshold is
 * applied to the sum of gradients. Gradients that exceed this threshold are then summed and counted and the
 * Focus metric is computed from these.
 *
 * --------------------------------------------------------------------------------------------------------------------
 */

#define CCI_CMD_VID_GET_FOCUS_METRIC_THRESHOLD	0x0314
#define CCI_CMD_VID_SET_FOCUS_METRIC_THRESHOLD	0x0315

cci_result_t CCI_GetVidFocusMetricThreshold(uint32_t* focus_metric_threshold)
{
	cci_result_t status = CCI_OK;
	cci_union_data_t rx[2];

	status = CCI_ReadCMD(CCI_CMD_VID_GET_FOCUS_METRIC_THRESHOLD, 2, rx);

	if (CCI_OK != status)
		return status;

	*focus_metric_threshold = ((uint32_t) (rx[1].uint16
			<< 16) | (uint32_t) rx[0].uint16);

	return CCI_WaitBusyClear();
}

cci_result_t CCI_SetVidFocusMetricThreshold(uint32_t focus_metric_threshold)
{
	uint32_t value = focus_metric_threshold;
	cci_union_data_t tx[2];

	tx[0].uint16 = value & 0xFFFF;
	tx[1].uint16 = (value >> 16) & 0xFFFF;

	return CCI_WriteCMD(CCI_CMD_VID_SET_FOCUS_METRIC_THRESHOLD, 2, tx);
}

/* --------------------------------------------------------------------------------------------------------------------
 *
 * VID Focus Metric
 *
 * This function returns the most recently calculated scene focus metric. The focus metric calculation counts image
 * gradients that exceed the focus metric threshold. Larger values imply better scene focus due the presence of more
 * large gradients. The focus metric is not defined if the video scene focus metric calculations are not enabled.
 * The focus metric uses the Tenengrad method, an edge-based metric that measures the sum of the horizontal and
 * vertical gradients using Sobel operators. The focus metric threshold is applied to the sum of gradients. Gradients
 * that exceed this threshold are then summed and counted and the Focus metric is computed from these.
 *
 * --------------------------------------------------------------------------------------------------------------------
 */

#define CCI_CMD_VID_GET_FOCUS_METRIC	0x0318

cci_result_t CCI_GetVidFocusMetric(uint32_t* focus_metric)
{
	cci_result_t status = CCI_OK;
	cci_union_data_t rx[2];

	status = CCI_ReadCMD(CCI_CMD_VID_GET_FOCUS_METRIC, 2, rx);

	if (CCI_OK != status)
		return status;

	*focus_metric = ((uint32_t) (rx[1].uint16
			<< 16) | (uint32_t) rx[0].uint16);

	return CCI_WaitBusyClear();
}

/* --------------------------------------------------------------------------------------------------------------------
 *
 * VID Video Freeze Enable State
 *
 * This function allows the current frame to be repeated in lieu of a live video stream. When enabled, live video is
 * halted from the camera. When disabled, live video resumes.
 *
 * --------------------------------------------------------------------------------------------------------------------
 */

#define CCI_CMD_VID_GET_FREEZE_ENABLE_STATE	0x0324
#define CCI_CMD_VID_SET_FREEZE_ENABLE_STATE	0x0325

cci_result_t CCI_GetVidFreezeEnableState(uint32_t* freeze_enable_state)
{
	cci_result_t status = CCI_OK;
	cci_union_data_t rx[2];

	status = CCI_ReadCMD(CCI_CMD_VID_GET_FREEZE_ENABLE_STATE, 2, rx);

	if (CCI_OK != status)
		return status;

	*freeze_enable_state = ((uint32_t) (rx[1].uint16
			<< 16) | (uint32_t) rx[0].uint16);

	return CCI_WaitBusyClear();
}

cci_result_t CCI_SetVidFreezeEnableState(uint32_t freeze_enable_state)
{
	uint32_t value = freeze_enable_state;
	cci_union_data_t tx[2];

	tx[0].uint16 = value & 0xFFFF;
	tx[1].uint16 = (value >> 16) & 0xFFFF;

	return CCI_WriteCMD(CCI_CMD_VID_SET_FREEZE_ENABLE_STATE, 2, tx);
}

/* --------------------------------------------------------------------------------------------------------------------
 *
 * VID Video Output Format
 *
 * This function provides the method to specify or retrieve the current video output format. In the current revision,
 * only RGB888 and RAW14 are valid formats.
 *
 * --------------------------------------------------------------------------------------------------------------------
 */

#define CCI_CMD_VID_GET_OUTPUT_FORMAT	0x0330
#define CCI_CMD_VID_SET_OUTPUT_FORMAT	0x0331

cci_result_t CCI_GetVidVideoOutputFormat(cci_vid_video_output_format_e* video_output_format)
{
	cci_result_t status = CCI_OK;
	cci_union_data_t rx[2];

	status = CCI_ReadCMD(CCI_CMD_VID_GET_OUTPUT_FORMAT, 2, rx);

	if (CCI_OK != status)
		return status;

	*video_output_format = (cci_vid_video_output_format_e) ((uint32_t) (rx[1].uint16 << 16) | (uint32_t) rx[0].uint16);

	return CCI_WaitBusyClear();
}

cci_result_t CCI_SetVidVideoOutputFormat(cci_vid_video_output_format_e video_output_format)
{
	uint32_t value = (uint32_t)video_output_format;
	cci_union_data_t tx[2];

	tx[0].uint16 = value & 0xFFFF;
	tx[1].uint16 = (value >> 16) & 0xFFFF;

	return CCI_WriteCMD(CCI_CMD_VID_SET_OUTPUT_FORMAT, 2, tx);
}

/* --------------------------------------------------------------------------------------------------------------------
 *
 * VID Low Gain Pseudo-Color Look-Up Table Select
 *
 * This function allows selection of the video output pseudo-color LUT to be used when the camera is in Low Gain
 * Mode. This LUT applies to the video processed by camera post AGC application before output. Color LUTs do not
 * apply to raw video output of any format. Requires using the video output format of 24-bit R, G, B, AGC
 * enabled and scaled to 8-bit output.
 *
 * --------------------------------------------------------------------------------------------------------------------
 */

#define CCI_CMD_VID_GET_LOW_GAIN_PCOLOR_LUT	0x0334
#define CCI_CMD_VID_SET_LOW_GAIN_PCOLOR_LUT	0x0335

cci_result_t CCI_GetVidLowGainPcolorLut(cci_pcolor_lut_e* pcolor_lut)
{
	cci_result_t status = CCI_OK;
	cci_union_data_t rx[2];

	status = CCI_ReadCMD(CCI_CMD_VID_GET_LOW_GAIN_PCOLOR_LUT, 2, rx);

	if (CCI_OK != status)
		return status;

	*pcolor_lut = (cci_vid_video_output_format_e) ((uint32_t) (rx[1].uint16 << 16) | (uint32_t) rx[0].uint16);

	return CCI_WaitBusyClear();
}

cci_result_t CCI_SetVidLowGainPcolorLut(cci_pcolor_lut_e pcolor_lut)
{
	uint32_t value = (uint32_t)pcolor_lut;
	cci_union_data_t tx[2];

	tx[0].uint16 = value & 0xFFFF;
	tx[1].uint16 = (value >> 16) & 0xFFFF;

	return CCI_WriteCMD(CCI_CMD_VID_SET_LOW_GAIN_PCOLOR_LUT, 2, tx);
}
