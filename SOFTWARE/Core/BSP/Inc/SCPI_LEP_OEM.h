/*
 * SCPI_LEP_OEM.h
 *
 *  Created on: Feb 22, 2024
 *      Author: grzegorz
 */

#ifndef BSP_INC_SCPI_LEP_OEM_H_
#define BSP_INC_SCPI_LEP_OEM_H_

#include "scpi/scpi.h"

scpi_result_t SCPI_LeptonOemPartNumberQ(scpi_t * context);

scpi_result_t SCPI_LeptonOemSoftwareRevisionQ(scpi_t * context);

scpi_result_t SCPI_LeptonOemVideoOutputEnableQ(scpi_t * context);
scpi_result_t SCPI_LeptonOemVideoOutputEnable(scpi_t * context);

scpi_result_t SCPI_LeptonOemVideoOutputFormatQ(scpi_t * context);
scpi_result_t SCPI_LeptonOemVideoOutputFormat(scpi_t * context);

scpi_result_t SCPI_LeptonOemVideoOutputSourceQ(scpi_t * context);
scpi_result_t SCPI_LeptonOemVideoOutputSource(scpi_t * context);

scpi_result_t SCPI_LeptonOemReboot(scpi_t * context);

scpi_result_t SCPI_LeptonOemStatus(scpi_t * context);

scpi_result_t SCPI_LeptonOemVideoOutputSourceConstantValueQ(scpi_t * context);
scpi_result_t SCPI_LeptonOemVideoOutputSourceConstantValue(scpi_t * context);

scpi_result_t SCPI_LeptonOemFFCNormalizationTargetQ(scpi_t * context);
scpi_result_t SCPI_LeptonOemFFCNormalizationTarget(scpi_t * context);
scpi_result_t SCPI_LeptonRunOemFFC(scpi_t * context);

scpi_result_t SCPI_LeptonOemFrameMeanIntensityQ(scpi_t * context);

scpi_result_t SCPI_LeptonOemShutterProfileQ(scpi_t * context);
scpi_result_t SCPI_LeptonOemShutterProfile(scpi_t * context);

scpi_result_t SCPI_LeptonOemThermalShutdownEnableQ(scpi_t * context);
scpi_result_t SCPI_LeptonOemThermalShutdownEnable(scpi_t * context);

scpi_result_t SCPI_LeptonOemBadPixelReplacmenetControlQ(scpi_t * context);
scpi_result_t SCPI_LeptonOemBadPixelReplacmenetControl(scpi_t * context);

scpi_result_t SCPI_LeptonOemTemporalFilterControlQ(scpi_t * context);
scpi_result_t SCPI_LeptonOemTemporalFilterControl(scpi_t * context);

scpi_result_t SCPI_LeptonOemColumnNoiseFilterControlQ(scpi_t * context);
scpi_result_t SCPI_LeptonOemColumnNoiseFilterControl(scpi_t * context);

scpi_result_t SCPI_LeptonOemPixelNoiseFilterControlQ(scpi_t * context);
scpi_result_t SCPI_LeptonOemPixelNoiseFilterControl(scpi_t * context);

#endif /* BSP_INC_SCPI_LEP_OEM_H_ */
