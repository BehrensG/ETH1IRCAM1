/*
 * SCPI_LEP_VID.h
 *
 *  Created on: Feb 27, 2024
 *      Author: BehrensG
 */

#ifndef BSP_INC_SCPI_LEP_VID_H_
#define BSP_INC_SCPI_LEP_VID_H_

#include "scpi/scpi.h"

scpi_result_t SCPI_LeptonVideoPColorLutQ(scpi_t * context);
scpi_result_t SCPI_LeptonVideoPColorLut(scpi_t * context);

scpi_result_t SCPI_LeptonVideoFocusCalculationEnableStateQ(scpi_t * context);
scpi_result_t SCPI_LeptonVideoFocusCalculationEnableState(scpi_t * context);

scpi_result_t SCPI_LeptonVideoFocusRoiQ(scpi_t * context);
scpi_result_t SCPI_LeptonVideoFocusRoi(scpi_t * context);

scpi_result_t SCPI_LeptonVideoFocusMetricThresholdQ(scpi_t * context);
scpi_result_t SCPI_LeptonVideoFocusMetricThreshold(scpi_t * context);

scpi_result_t SCPI_LeptonVideoFocusMetricQ(scpi_t * context);

scpi_result_t SCPI_LeptonVideoFreezeEnableStateQ(scpi_t * context);
scpi_result_t SCPI_LeptonVideoFreezeEnableState(scpi_t * context);

scpi_result_t SCPI_LeptonVideoOutputFormatQ(scpi_t * context);
scpi_result_t SCPI_LeptonVideoOutputFormat(scpi_t * context);

scpi_result_t SCPI_LeptonVideoTimeoutQ(scpi_t * context);
scpi_result_t SCPI_LeptonVideoTimeout(scpi_t * context);

#endif /* BSP_INC_SCPI_LEP_VID_H_ */
