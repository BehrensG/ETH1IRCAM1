/*
 * CCI_RAD.h
 *
 *  Created on: Feb 28, 2024
 *      Author: grzegorz
 */

#ifndef BSP_LEPTON_INC_CCI_RAD_H_
#define BSP_LEPTON_INC_CCI_RAD_H_

#include "CCI.h"

typedef enum
{
	CCI_RAD_DISABLE = 0,
	CCI_RAD_ENABLE

}cci_rad_enable_e;

typedef enum CCI_RAD_TS_MODE_E_TAG
{
	CCI_RAD_TS_USER_MODE = 0,
	CCI_RAD_TS_CAL_MODE,
	CCI_RAD_TS_FIXED_MODE

}cci_rad_ts_mode_e;

typedef enum
{
	CCI_RAD_STATUS_ERROR = -1,
	CCI_RAD_STATUS_READY = 0,
	CCI_RAD_STATUS_BUSY,
	CCI_RAD_FRAME_AVERAGE_COLLECTING_FRAMES

} cci_rad_status_e;

typedef struct
{
	uint16_t sceneEmissivity; /* 3.13 */
	uint16_t TBkgK; /* 16.0 value in Kelvin 100x */
	uint16_t tauWindow; /* 3.13 */
	uint16_t TWindowK; /* 16.0 value in Kelvin 100x */
	uint16_t tauAtm; /* 3.13 */
	uint16_t TAtmK; /* 16.0 value in Kelvin 100x */
	uint16_t reflWindow; /* 3.13 */
	uint16_t TReflK; /* 16.0 value in Kelvin 100x */

}cci_rad_flux_linear_params_t;

typedef struct
{
	uint16_t startRow;
	uint16_t startCol;
	uint16_t endRow;
	uint16_t endCol;

}cci_rad_roi_t;

typedef struct
{
	uint16_t radSpotmeterValue;
	uint16_t radSpotmeterMaxValue;
	uint16_t radSpotmeterMinValue;
	uint16_t radSpotmeterPopulation;

}cci_rad_spotmeter_obj_kelvin_t;

typedef struct
{
	uint32_t RBFO_R; // value is not scaled
	uint32_t RBFO_B; // value is scaled by X << n
	uint32_t RBFO_F;
	int32_t RBFO_O;

}cci_rbfo_t;

cci_result_t CCI_GetRadTLinearEnableState(cci_rad_enable_e* rad_tlinear_enable_state);
cci_result_t CCI_SetRadTLinearEnableState(cci_rad_enable_e rad_tlinear_enable_state);

#endif /* BSP_LEPTON_INC_CCI_RAD_H_ */
