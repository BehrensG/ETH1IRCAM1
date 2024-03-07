/*
 * CCI_RAD.c
 *
 *  Created on: Feb 28, 2024
 *      Author: grzegorz
 */


#include "CCI_RAD.h"

/* --------------------------------------------------------------------------------------------------------------------
 *
 * RAD Radiometry Control Enable
 *
 * This function enables or disables the Camera Radiometry Control (temperature stable output), or returns the state
 * of Control.
 *
 * --------------------------------------------------------------------------------------------------------------------
 */

#define CCI_CMD_RAD_GET_RADIOMETRY_CONTROL_ENABLE	0x4E10
#define CCI_CMD_RAD_SET_RADIOMETRY_CONTROL_ENABLE	0x4E11

cci_result_t CCI_GetRadEnableState(cci_rad_enable_e* radiometry_control_enable)
{
	cci_result_t status = CCI_OK;
	cci_union_data_t rx[2];

	status = CCI_ReadCMD(CCI_CMD_RAD_GET_RADIOMETRY_CONTROL_ENABLE, 2, rx);

	if (CCI_OK != status)
		return status;

	*radiometry_control_enable = (cci_rad_enable_e) ((uint32_t) (rx[1].uint16
			<< 16) | (uint32_t) rx[0].uint16);

	return CCI_WaitBusyClear();
}

cci_result_t CCI_SetRadEnableState(cci_rad_enable_e radiometry_control_enable)
{
	uint32_t value = (uint32_t)radiometry_control_enable;
	cci_union_data_t tx[2];

	tx[0].uint16 = value & 0xFFFF;
	tx[1].uint16 = (value >> 16) & 0xFFFF;

	return CCI_WriteCMD(CCI_CMD_RAD_SET_RADIOMETRY_CONTROL_ENABLE, 2, tx);
}

/* --------------------------------------------------------------------------------------------------------------------
 *
 * RAD TShutter Mode
 *
 * This function gets or sets the TShutter mode. The TShutter mode specifies how TShutter value is obtained at FFC.
 *	User: Use the TShutter value set with LEP_SetRadTShutter()
 *	Cal: Use TEqShutter from calibration
 *	Fixed: the shutter temperature is considered static, and therefore the spotmeter is not updated at FFC
 *
 * --------------------------------------------------------------------------------------------------------------------
 */

#define CCI_CMD_RAD_GET_RADIOMETRY_TSHUTTER_MODE	0x4E24
#define CCI_CMD_RAD_SET_RADIOMETRY_TSHUTTER_MODE	0x4E25

cci_result_t CCI_GetRadTShutterMode(cci_rad_ts_mode_e* radiometry_tshutter_mode)
{
	cci_result_t status = CCI_OK;
	cci_union_data_t rx[2];

	status = CCI_ReadCMD(CCI_CMD_RAD_GET_RADIOMETRY_TSHUTTER_MODE, 2, rx);

	if (CCI_OK != status)
		return status;

	*radiometry_tshutter_mode = (cci_rad_ts_mode_e) ((uint32_t) (rx[1].uint16
			<< 16) | (uint32_t) rx[0].uint16);

	return CCI_WaitBusyClear();
}

cci_result_t CCI_SetRadTShutterMode(cci_rad_ts_mode_e radiometry_tshutter_mode)
{
	uint32_t value = (uint32_t)radiometry_tshutter_mode;
	cci_union_data_t tx[2];

	tx[0].uint16 = value & 0xFFFF;
	tx[1].uint16 = (value >> 16) & 0xFFFF;

	return CCI_WriteCMD(CCI_CMD_RAD_SET_RADIOMETRY_TSHUTTER_MODE, 2, tx);
}

/* --------------------------------------------------------------------------------------------------------------------
 *
 * RAD TShutter Temperature
 *
 * This function gets or sets the TShutter temperature. The TShutter temperature is used at FFC when the TShutter
 * Mode is “User”.
 *
 * --------------------------------------------------------------------------------------------------------------------
 */

#define CCI_CMD_RAD_GET_RADIOMETRY_TSHUTTER_TEMPERATURE		0x4E28
#define CCI_CMD_RAD_SET_RADIOMETRY_TSHUTTER_TEMPERATURE		0x4E29

cci_result_t CCI_GetRadTShutter(uint16_t* radiometry_tshutter_mode)
{
	cci_result_t status = CCI_OK;
	cci_union_data_t rx;

	status = CCI_ReadCMD(CCI_CMD_RAD_GET_RADIOMETRY_TSHUTTER_MODE, 1, &rx);

	if (CCI_OK != status)
		return status;

	*radiometry_tshutter_mode = rx.uint16;

	return CCI_WaitBusyClear();
}

cci_result_t CCI_SetRadTShutter(uint16_t radiometry_tshutter_mode)
{
	cci_union_data_t tx;

	tx.uint16 = radiometry_tshutter_mode;

	return CCI_WriteCMD(CCI_CMD_RAD_SET_RADIOMETRY_TSHUTTER_MODE, 2, &tx);
}

/* --------------------------------------------------------------------------------------------------------------------
 *
 * RAD FFC Normalization
 *
 * This command executes a Flat-Field Correction (FFC) and updates the Global Gain and Global Offset. The target value
 * is factory set and should not be changed under normal circumstances. The Run command executes an FFC using currently
 * active values for the FFC normalization target and number of frames to average.
 * This command executes synchronously. Poll the RAD Run Status to determine when this command completes.
 *
 * --------------------------------------------------------------------------------------------------------------------
 */

#define CCI_CMD_RAD_GET_RADIOMETRY_FCC_RUN	0x4E2E

cci_result_t CCI_RunRadFFC(void)
{
	CCI_WriteRegister(CCI_REG_COMMAND, CCI_CMD_RAD_GET_RADIOMETRY_FCC_RUN);

	return CCI_WaitBusyClear();
}

/* --------------------------------------------------------------------------------------------------------------------
 *
 * RAD Run Status
 *
 * This function obtains the current status of a RAD module run operation. This function is used whenever a RAD command
 * is issued that executes an operation like the run FFC. Typically, the host polls the status to determine when the
 * command has completed. If the return value is negative, then the operation completed with an error.
 * Positive values indicate an in-process state.
 *
 * --------------------------------------------------------------------------------------------------------------------
 */

#define CCI_CMD_RAD_GET_RADIOMETRY_RUN_STATUS	0x4E30

cci_result_t CCI_GetRadRunStatus(cci_rad_status_e* rad_status)
{
	cci_result_t status = CCI_OK;
	cci_union_data_t rx[2];

	status = CCI_ReadCMD(CCI_CMD_RAD_GET_RADIOMETRY_RUN_STATUS, 2, rx);

	if (CCI_OK != status)
		return status;

	*rad_status = (cci_rad_status_e) ((uint32_t) (rx[1].uint16
			<< 16) | (uint32_t) rx[0].uint16);

	return CCI_WaitBusyClear();
}

/* --------------------------------------------------------------------------------------------------------------------
 *
 * RAD Flux Linear Parameters
 *
 * These functions either get or set various scene parameters used in the T-Linear calculations.
 *
 * --------------------------------------------------------------------------------------------------------------------
 */

#define CCI_CMD_RAD_GET_RADIOMETRY_FLUX_LINEAR_PARAMS	0x4EBC
#define CCI_CMD_RAD_SET_RADIOMETRY_FLUX_LINEAR_PARAMS	0x4EBD

cci_result_t CCI_GetRadFluxLinearParams(cci_rad_flux_linear_params_t* rad_flux_linear_params)
{
	cci_result_t status = CCI_OK;
	cci_union_data_t rx[8];

	status = CCI_ReadCMD(CCI_CMD_RAD_GET_RADIOMETRY_FLUX_LINEAR_PARAMS, 8, rx);

	if (CCI_OK != status)
		return status;

	rad_flux_linear_params->sceneEmissivity = rx[0].uint16;
	rad_flux_linear_params->TBkgK = rx[1].uint16;
	rad_flux_linear_params->tauWindow = rx[2].uint16;
	rad_flux_linear_params->TWindowK = rx[3].uint16;
	rad_flux_linear_params->tauAtm = rx[4].uint16;
	rad_flux_linear_params->TAtmK = rx[5].uint16;
	rad_flux_linear_params->reflWindow = rx[6].uint16;
	rad_flux_linear_params->TReflK = rx[7].uint16;

	return CCI_WaitBusyClear();
}

cci_result_t CCI_SetRadFluxLinearParams(cci_rad_flux_linear_params_t rad_flux_linear_params)
{
	cci_union_data_t tx[8];

	tx[0].uint16 = rad_flux_linear_params.sceneEmissivity;
	tx[1].uint16 = rad_flux_linear_params.TBkgK;
	tx[2].uint16 = rad_flux_linear_params.tauWindow;
	tx[3].uint16 = rad_flux_linear_params.TWindowK;
	tx[4].uint16 = rad_flux_linear_params.tauAtm;
	tx[5].uint16 = rad_flux_linear_params.TAtmK;
	tx[6].uint16 = rad_flux_linear_params.reflWindow;
	tx[7].uint16 = rad_flux_linear_params.TReflK;

	return CCI_WriteCMD(CCI_CMD_RAD_GET_RADIOMETRY_FLUX_LINEAR_PARAMS, 8, tx);
}

/* --------------------------------------------------------------------------------------------------------------------
 *
 * RAD T-Linear Enable State
 *
 * These functions either get or set the T-Linear output enable state. When enabled, the video output represents
 * temperature in Kelvin with some scale factor defined by the T-linear Resolution parameter. T-Linear mode requires
 * radiometry mode (temperature stable output) to also be enabled.
 *
 * --------------------------------------------------------------------------------------------------------------------
 */

#define CCI_CMD_RAD_GET_TLINEAR_ENABLE_STATE	0x4EC0
#define CCI_CMD_RAD_SET_TLINEAR_ENABLE_STATE	0x4EC1

cci_result_t CCI_GetRadTLinearEnableState(cci_rad_enable_e* rad_tlinear_enable_state)
{
	cci_result_t status = CCI_OK;
	cci_union_data_t rx[2];

	status = CCI_ReadCMD(CCI_CMD_RAD_GET_TLINEAR_ENABLE_STATE, 2, rx);

	if (CCI_OK != status)
		return status;

	*rad_tlinear_enable_state = (cci_rad_enable_e) ((uint32_t) (rx[1].uint16
			<< 16) | (uint32_t) rx[0].uint16);

	return CCI_WaitBusyClear();
}

cci_result_t CCI_SetRadTLinearEnableState(cci_rad_enable_e rad_tlinear_enable_state)
{
	uint32_t value = (uint32_t)rad_tlinear_enable_state;
	cci_union_data_t tx[2];

	tx[0].uint16 = value & 0xFFFF;
	tx[1].uint16 = (value >> 16) & 0xFFFF;

	return CCI_WriteCMD(CCI_CMD_RAD_SET_TLINEAR_ENABLE_STATE, 2, tx);
}

/* --------------------------------------------------------------------------------------------------------------------
 *
 * RAD T-Linear Resolution
 *
 * These functions either get or set the T-Linear output resolution, which defines the scale factor for the temperature
 * measurements contained in the video output (per-pixel) with T-Linear mode enabled.
 *
 * --------------------------------------------------------------------------------------------------------------------
 */

// TBD

/* --------------------------------------------------------------------------------------------------------------------
 *
 * RAD T-Linear Auto Resolution
 *
 * These functions either get or set the T-Linear automatic resolution enable state. When enabled, T-Linear output
 * resolution is chosen automatically based on scene statistics and gain mode.
 *
 * --------------------------------------------------------------------------------------------------------------------
 */

#define CCI_CMD_RAD_GET_TLINEAR_AUTO_RESOLUTION		0x4EC8
#define CCI_CMD_RAD_SET_TLINEAR_AUTO_RESOLUTION		0x4EC9

cci_result_t CCI_GetRadTLinearAutoResolution(cci_rad_enable_e* rad_tlinear_auto_resolution)
{
	cci_result_t status = CCI_OK;
	cci_union_data_t rx[2];

	status = CCI_ReadCMD(CCI_CMD_RAD_GET_TLINEAR_AUTO_RESOLUTION, 2, rx);

	if (CCI_OK != status)
		return status;

	*rad_tlinear_auto_resolution = (cci_rad_enable_e) ((uint32_t) (rx[1].uint16
			<< 16) | (uint32_t) rx[0].uint16);

	return CCI_WaitBusyClear();
}

cci_result_t CCI_SetRadTLinearAutoResolution(cci_rad_enable_e rad_tlinear_auto_resolution)
{
	uint32_t value = (uint32_t)rad_tlinear_auto_resolution;
	cci_union_data_t tx[2];

	tx[0].uint16 = value & 0xFFFF;
	tx[1].uint16 = (value >> 16) & 0xFFFF;

	return CCI_WriteCMD(CCI_CMD_RAD_SET_TLINEAR_AUTO_RESOLUTION, 2, tx);
}

/* --------------------------------------------------------------------------------------------------------------------
 *
 * RAD Spotmeter Region of Interest (ROI)
 *
 * These functions either get or set a rectangular region of interest within the video frame extents which RAD
 * operations can use to calculate temperature measurement minimum, maximum, and average.
 *
 * --------------------------------------------------------------------------------------------------------------------
 */

#define CCI_CMD_RAD_GET_SPOTMETER_ROI	0x4ECC
#define CCI_CMD_RAD_SET_SPOTMETER_ROI	0x4ECD

cci_result_t CCI_GetRadSpotmeterRoi(cci_rad_roi_t* rad_roi)
{
	cci_result_t status = CCI_OK;
	cci_union_data_t rx[4];

	status = CCI_ReadCMD(CCI_CMD_RAD_GET_RADIOMETRY_FLUX_LINEAR_PARAMS, 4, rx);

	if (CCI_OK != status)
		return status;

	rad_roi->startRow = rx[0].uint16;
	rad_roi->startCol = rx[1].uint16;
	rad_roi->endRow = rx[2].uint16;
	rad_roi->endCol = rx[3].uint16;

	return CCI_WaitBusyClear();
}

cci_result_t CCI_SetRadSpotmeterRoi(cci_rad_roi_t rad_roi)
{
	cci_union_data_t tx[4];

	tx[0].uint16 = rad_roi.startRow;
	tx[1].uint16 = rad_roi.startCol;
	tx[2].uint16 = rad_roi.endRow;
	tx[3].uint16 = rad_roi.endCol;

	return CCI_WriteCMD(CCI_CMD_RAD_SET_TLINEAR_ENABLE_STATE, 4, tx);
}

/* --------------------------------------------------------------------------------------------------------------------
 *
 * RAD Spotmeter Value
 *
 * These functions get the mean, min, and max temperature values for pixels within the spotmeter ROI.
 *
 * --------------------------------------------------------------------------------------------------------------------
 */

#define CCI_CMD_RAD_GET_SPOTMETER_ROI	0x4ECC
#define CCI_CMD_RAD_SET_SPOTMETER_ROI	0x4ECD

cci_result_t CCI_GetRadSpotmeterObjInKelvinX100(cci_rad_roi_t* rad_roi)
{
	cci_result_t status = CCI_OK;
	cci_union_data_t rx[4];

	status = CCI_ReadCMD(CCI_CMD_RAD_GET_RADIOMETRY_FLUX_LINEAR_PARAMS, 4, rx);

	if (CCI_OK != status)
		return status;

	rad_roi->startRow = rx[0].uint16;
	rad_roi->startCol = rx[1].uint16;
	rad_roi->endRow = rx[2].uint16;
	rad_roi->endCol = rx[3].uint16;

	return CCI_WaitBusyClear();
}

/* --------------------------------------------------------------------------------------------------------------------
 *
 * RAD Low Gain RBFO External Parameters
 *
 * These functions either get or set the radiometry low gain RBFO External parameters. The RBFO parameters define the
 * equation for conversion between flux and temperature.
 *
 * --------------------------------------------------------------------------------------------------------------------
 */

#define CCI_CMD_RAD_GET_LOW_GAIN_RBFO_EXTERNA_PARAMETERS	0x4ECC
#define CCI_CMD_RAD_SET_LOW_GAIN_RBFO_EXTERNA_PARAMETERS	0x4ECD

cci_result_t CCI_GetRadExternalRBFOLowGain(cci_rbfo_t* rbfo)
{
	cci_result_t status = CCI_OK;
	cci_union_data_t rx[8];

	status = CCI_ReadCMD(CCI_CMD_RAD_GET_LOW_GAIN_RBFO_EXTERNA_PARAMETERS, 8, rx);

	if (CCI_OK != status)
		return status;

	rbfo->RBFO_R = (uint32_t)(rx[1].uint16 << 16) | (uint32_t)rx[0].uint16;
	rbfo->RBFO_B = (uint32_t)(rx[3].uint16 << 16) | (uint32_t)rx[2].uint16;
	rbfo->RBFO_F = (uint32_t)(rx[5].uint16 << 16) | (uint32_t)rx[4].uint16;
	rbfo->RBFO_O = (int32_t)((uint32_t)(rx[1].uint16 << 16) | (uint32_t)rx[0].uint16);

	return CCI_WaitBusyClear();
}

cci_result_t CCI_SetRadExternalRBFOLowGain(cci_rbfo_t rbfo)
{
	cci_union_data_t tx[8];

	tx[0].uint16 = rbfo.RBFO_R & 0xFFFF;
	tx[1].uint16 = (rbfo.RBFO_R >> 16) & 0xFFFF;

	tx[2].uint16 = rbfo.RBFO_B & 0xFFFF;
	tx[3].uint16 = (rbfo.RBFO_B >> 16) & 0xFFFF;

	tx[4].uint16 = rbfo.RBFO_F & 0xFFFF;
	tx[5].uint16 = (rbfo.RBFO_F >> 16) & 0xFFFF;

	tx[6].uint16 = rbfo.RBFO_O & 0xFFFF;
	tx[7].uint16 = (rbfo.RBFO_O >> 16) & 0xFFFF;

	return CCI_WriteCMD(CCI_CMD_RAD_SET_LOW_GAIN_RBFO_EXTERNA_PARAMETERS, 8, tx);
}
