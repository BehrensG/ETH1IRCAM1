/*
 * SCPI_Lepton_SYS.h
 *
 *  Created on: Feb 15, 2024
 *      Author: grzegorz
 */

#ifndef BSP_INC_SCPI_LEP_SYS_H_
#define BSP_INC_SCPI_LEP_SYS_H_

#include "scpi/scpi.h"

scpi_result_t SCPI_LeptonSystemSystemStatusQ (scpi_t * context);

scpi_result_t SCPI_LeptonSystemSerialNumberQ(scpi_t * context);

scpi_result_t SCPI_LeptonSystemUpTimeQ(scpi_t * context);

scpi_result_t SCPI_LeptonSystemAUXTemperatureQ(scpi_t * context);

scpi_result_t SCPI_LeptonSystemFPATemperatureQ(scpi_t * context);

scpi_result_t SCPI_LeptonSystemTelemetryEnableStateQ(scpi_t * context);
scpi_result_t SCPI_LeptonSystemTelemetryEnableState(scpi_t * context);

scpi_result_t SCPI_LeptonSystemFrameAverageRun(scpi_t * context);
scpi_result_t SCPI_LeptonSystemFrameToAverageQ(scpi_t * context);
scpi_result_t SCPI_LeptonSystemFrameToAverage(scpi_t * context);

scpi_result_t SCPI_LeptonSystemSceneStatisticsQ(scpi_t * context);

scpi_result_t SCPI_LeptonSystemSceneRoiQ(scpi_t * context);
scpi_result_t SCPI_LeptonSystemSceneRoi(scpi_t * context);

scpi_result_t SCPI_LeptonOemThermalShutdownCountQ(scpi_t * context);

scpi_result_t SCPI_LeptonSystemShutterPositionQ(scpi_t * context);
scpi_result_t SCPI_LeptonSystemShutterPosition(scpi_t * context);

scpi_result_t SCPI_LeptonSystemFFCStatusQ(scpi_t * context);

scpi_result_t SCPI_LeptonSystemFFCNormalization(scpi_t * context);

scpi_result_t SCPI_LeptonSystemGainModeQ(scpi_t * context);
scpi_result_t SCPI_LeptonSystemGainMode(scpi_t * context);

#endif /* BSP_INC_SCPI_LEP_SYS_H_ */
