/*-
 * Copyright (c) 2012-2013 Jan Breuer,
 *
 * All Rights Reserved
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are
 * met:
 * 1. Redistributions of source code must retain the above copyright notice,
 *    this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY THE AUTHORS ``AS IS'' AND ANY EXPRESS OR
 * IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE AUTHORS OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR
 * BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
 * WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE
 * OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN
 * IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

/**
 * @file   scpi-def.c
 * @date   Thu Nov 15 10:58:45 UTC 2012
 *
 * @brief  SCPI parser test
 *
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <cmsis_os.h>


#include "main.h"
#include "scpi/scpi.h"

#include "SCPI_Def.h"
#include "SCPI_System.h"

#include "SCPI_LEP_SYS.h"
#include "SCPI_LEP_OEM.h"
#include "SCPI_LEP_IMG.h"
#include "SCPI_LEP_VID.h"


scpi_choice_def_t scpi_boolean_select[] =
{
    {"OFF", 0},
    {"ON", 1},
	{"0", 0},
	{"1", 1},
    SCPI_CHOICE_LIST_END
};



static scpi_result_t SCPI_Rst(scpi_t * context)
{
	HAL_NVIC_SystemReset();
    return SCPI_RES_OK;
}

static scpi_result_t SCPI_IdnQ(scpi_t * context)
{
	int32_t ptr = 0;
	static char info[46] = {'\0'};

    for (uint8_t i = 0; i < 4; i++)
    {
        if (context->idn[i])
        {
        	ptr += snprintf(info + ptr, sizeof(info) - ptr, "%s,", context->idn[i] );
        }
        else{}
    }

    SCPI_ResultCharacters(context, info, 45);
    return SCPI_RES_OK;
}


scpi_result_t SCPI_SystemErrorQ(scpi_t * context)
{
	scpi_error_t error;
	SCPI_ErrorPop(context, &error);
	SCPI_ResultError(context, &error);
#if USE_DEVICE_DEPENDENT_ERROR_INFORMATION
	SCPIDEFINE_free(&context->error_info_heap, error.device_dependent_info, false);
#endif

    return SCPI_RES_OK;
}

scpi_result_t SCPI_TS(scpi_t * context)
{

    return SCPI_RES_OK;
}

const scpi_command_t scpi_commands[] = {
    /* IEEE Mandated Commands (SCPI std V1999.0 4.1.1) */
    { .pattern = "*CLS", .callback = SCPI_CoreCls,},
    { .pattern = "*ESE", .callback = SCPI_CoreEse,},
    { .pattern = "*ESE?", .callback = SCPI_CoreEseQ,},
    { .pattern = "*ESR?", .callback = SCPI_CoreEsrQ,},
    { .pattern = "*IDN?", .callback = SCPI_IdnQ,},
    { .pattern = "*OPC", .callback = SCPI_CoreOpc,},
    { .pattern = "*OPC?", .callback = SCPI_CoreOpcQ,},
    { .pattern = "*RST", .callback = SCPI_Rst,},
    { .pattern = "*SRE", .callback = SCPI_CoreSre,},
    { .pattern = "*SRE?", .callback = SCPI_CoreSreQ,},
    { .pattern = "*STB?", .callback = SCPI_CoreStbQ,},
    { .pattern = "*TST?", .callback = SCPI_CoreTstQ,},
    { .pattern = "*WAI", .callback = SCPI_CoreWai,},

    {.pattern = "STATus:QUEStionable[:EVENt]?", .callback = SCPI_StatusQuestionableEventQ,},
    /* {.pattern = "STATus:QUEStionable:CONDition?", .callback = scpi_stub_callback,}, */
    {.pattern = "STATus:QUEStionable:ENABle", .callback = SCPI_StatusQuestionableEnable,},
    {.pattern = "STATus:QUEStionable:ENABle?", .callback = SCPI_StatusQuestionableEnableQ,},

    {.pattern = "STATus:PRESet", .callback = SCPI_StatusPreset,},

    /* Required SCPI commands (SCPI std V1999.0 4.2.1) */
    {.pattern = "SYSTem:ERRor[:NEXT]?", .callback = SCPI_SystemErrorQ,},
    {.pattern = "SYSTem:ERRor:COUNt?", .callback = SCPI_SystemErrorCountQ,},
    {.pattern = "SYSTem:VERSion?", .callback = SCPI_SystemVersionQ,},
	{.pattern = "SYSTem:COMMunicate:LAN:IPADdress", .callback = SCPI_SystemCommunicateLanIpAddress,},
	{.pattern = "SYSTem:COMMunicate:LAN:IPADdress?", .callback = SCPI_SystemCommunicateLanIpAddressQ,},
	{.pattern = "SYSTem:COMMunicate:LAN:SMASk", .callback = SCPI_SystemCommunicateLanIpSmask,},
	{.pattern = "SYSTem:COMMunicate:LAN:SMASk?", .callback = SCPI_SystemCommunicateLanIpSmaskQ,},
	{.pattern = "SYSTem:COMMunicate:LAN:GATEway", .callback = SCPI_SystemCommunicateLanGateway,},
	{.pattern = "SYSTem:COMMunicate:LAN:GATEway?", .callback = SCPI_SystemCommunicateLanGatewayQ,},
	{.pattern = "SYSTem:COMMunicate:LAN:MAC", .callback = SCPI_SystemCommunicateLanMac,},
	{.pattern = "SYSTem:COMMunicate:LAN:MAC?", .callback = SCPI_SystemCommunicateLanMacQ,},
	{.pattern = "SYSTem:COMMunicate:LAN:TCPIP:PORT", .callback = SCPI_SystemCommunicateLanTcpIpPort,},
	{.pattern = "SYSTem:COMMunicate:LAN:TCPIP:PORT?", .callback = SCPI_SystemCommunicateLanTcpIpPortQ,},
	{.pattern = "SYSTem:COMMunicate:LAN:UDP:PORT", .callback = SCPI_SystemCommunicateLanUdpPort,},
	{.pattern = "SYSTem:COMMunicate:LAN:UDP:PORT?", .callback = SCPI_SystemCommunicateLanUdpPortQ,},
	{.pattern = "SYSTem:COMMunicate:LAN:UPDate", .callback = SCPI_SystemCommunicationLanUpdate,},
	{.pattern = "SYSTem:TEMPerature:UNIT", .callback = SCPI_SystemTemperatureUnit,},
	{.pattern = "SYSTem:TEMPerature:UNIT?", .callback = SCPI_SystemTemperatureUnitQ,},
	{.pattern = "SYSTem:SERVice:EEPROM", .callback = SCPI_SystemServiceEeprom,},
	{.pattern = "SYSTem:SERVice:STORe", .callback = SCPI_SystemServiceStore,},

	{.pattern = "LEPton:SYSTem:STATus?", .callback = SCPI_LeptonSystemSystemStatusQ,},
	{.pattern = "LEPton:SYSTem:SN?", .callback = SCPI_LeptonSystemSerialNumberQ,},
	{.pattern = "LEPton:SYSTem:UPTIMe?", .callback = SCPI_LeptonSystemUpTimeQ,},
	{.pattern = "LEPton:SYSTem:AUX:TEMPerature?", .callback = SCPI_LeptonSystemAUXTemperatureQ,},
	{.pattern = "LEPton:SYSTem:FPA:TEMPerature?", .callback = SCPI_LeptonSystemFPATemperatureQ,},
	{.pattern = "LEPton:SYSTem:FFC:NORMalization", .callback = SCPI_LeptonSystemFFCNormalization,},
	//{.pattern = "LEPton:SYSTem:TELEmetry[:ENAble]?", .callback = SCPI_LeptonSystemTelemetryEnableStateQ,},
	//{.pattern = "LEPton:SYSTem:TELEmetry[:ENAble]", .callback = SCPI_LeptonSystemTelemetryEnableState,},
	{.pattern = "LEPton:SYSTem:FRAMe:AVERage:RUN", .callback = SCPI_LeptonSystemFrameAverageRun,},
	{.pattern = "LEPton:SYSTem:FRAMe:AVERage?", .callback = SCPI_LeptonSystemFrameToAverageQ,},
	{.pattern = "LEPton:SYSTem:FRAMe:AVERage", .callback = SCPI_LeptonSystemFrameToAverage,},
	{.pattern = "LEPton:SYSTem:SCENE:STATistics?", .callback = SCPI_LeptonSystemSceneStatisticsQ,},
	{.pattern = "LEPton:SYSTem:SCENE:ROI?", .callback = SCPI_LeptonSystemSceneRoiQ,},
	{.pattern = "LEPton:SYSTem:SCENE:ROI?", .callback = SCPI_LeptonSystemSceneRoi,},
	{.pattern = "LEPton:SYSTem:THERmal:SHUTDown:COUNt?", .callback = SCPI_LeptonOemThermalShutdownCountQ,},
	{.pattern = "LEPton:SYSTem:FFC:NORMalization", .callback = SCPI_LeptonSystemFFCNormalization,},
	{.pattern = "LEPton:SYSTem:GAIN:MODe?", .callback = SCPI_LeptonSystemGainModeQ,},
	{.pattern = "LEPton:SYSTem:GAIN:MODe", .callback = SCPI_LeptonSystemGainMode,},
	{.pattern = "LEPton:SYSTem:SHUTTer:POSition?", .callback = SCPI_LeptonSystemShutterPositionQ,},
	{.pattern = "LEPton:SYSTem:SHUTTer:POSition", .callback = SCPI_LeptonSystemShutterPosition,},
	{.pattern = "LEPton:SYSTem:FFC:STATus", .callback = SCPI_LeptonSystemFFCStatusQ,},

	{.pattern = "LEPton:OEM:PART:NUMber?", .callback = SCPI_LeptonOemPartNumberQ,},
	{.pattern = "LEPton:OEM:SW:REVision?", .callback = SCPI_LeptonOemSoftwareRevisionQ,},
	{.pattern = "LEPton:OEM:VIDeo:OUTput:ENAble?", .callback = SCPI_LeptonOemVideoOutputEnableQ,},
	{.pattern = "LEPton:OEM:VIDeo:OUTput:ENAble", .callback = SCPI_LeptonOemVideoOutputEnable,},
	{.pattern = "LEPton:OEM:VIDeo:OUTput:FORMat?", .callback = SCPI_LeptonOemVideoOutputFormatQ,},
	{.pattern = "LEPton:OEM:VIDeo:OUTput:FORMat", .callback = SCPI_LeptonOemVideoOutputFormat,},
	{.pattern = "LEPton:OEM:VIDeo:OUTput:SOURce?", .callback = SCPI_LeptonOemVideoOutputSourceQ,},
	{.pattern = "LEPton:OEM:VIDeo:OUTput:SOURce?", .callback = SCPI_LeptonOemVideoOutputSource,},
	{.pattern = "LEPton:OEM:REBOOT", .callback = SCPI_LeptonOemReboot,},
	{.pattern = "LEPton:OEM[:STATus]?", .callback = SCPI_LeptonOemStatus,},
	{.pattern = "LEPton:OEM:OUTput:SOURce:CONSTant:VALue?", .callback = SCPI_LeptonOemVideoOutputSourceConstantValueQ,},
	{.pattern = "LEPton:OEM:OUTput:SOURce:CONSTant:VALue", .callback = SCPI_LeptonOemVideoOutputSourceConstantValue,},
	{.pattern = "LEPton:OEM:FFC:NORMalization:TARGet?", .callback = SCPI_LeptonOemFFCNormalizationTargetQ,},
	{.pattern = "LEPton:OEM:FFC:NORMalization:TARGet?", .callback = SCPI_LeptonOemFFCNormalizationTarget,},
	{.pattern = "LEPton:OEM:FFC:RUN", .callback = SCPI_LeptonRunOemFFC,},
	{.pattern = "LEPton:OEM:FRAMe:MEAN:INTensity", .callback = SCPI_LeptonOemFrameMeanIntensityQ,},
	{.pattern = "LEPton:OEM:SHUTTer:PROFile?", .callback = SCPI_LeptonOemShutterProfileQ,},
	{.pattern = "LEPton:OEM:SHUTTer:PROFile", .callback = SCPI_LeptonOemShutterProfile,},
	{.pattern = "LEPton:OEM:THERmal:SHUTDown[:ENAble]?", .callback = SCPI_LeptonOemThermalShutdownEnableQ,},
	{.pattern = "LEPton:OEM:THERmal:SHUTDown[:ENAble]", .callback = SCPI_LeptonOemThermalShutdownEnable,},
	{.pattern = "LEPton:OEM:BAD:PIXel:REPlacement:CONTrol[:ENAble]?", .callback = SCPI_LeptonOemBadPixelReplacmenetControlQ,},
	{.pattern = "LEPton:OEM:BAD:PIXel:REPlacement:CONTrol[:ENAble]", .callback = SCPI_LeptonOemBadPixelReplacmenetControl,},
	{.pattern = "LEPton:OEM:TEMPoral:FILter:CONTrol[:ENAble]?", .callback = SCPI_LeptonOemTemporalFilterControlQ,},
	{.pattern = "LEPton:OEM:TEMPoral:FILter:CONTrol[:ENAble]", .callback = SCPI_LeptonOemTemporalFilterControl,},
	{.pattern = "LEPton:OEM:COLumn:NOISE:FILter:CONTrol[:ENAble]?", .callback = SCPI_LeptonOemColumnNoiseFilterControlQ,},
	{.pattern = "LEPton:OEM:COLumn:NOISE:FILter:CONTrol[:ENAble]", .callback = SCPI_LeptonOemColumnNoiseFilterControl,},
	{.pattern = "LEPton:OEM:PIXel:NOISE:FILter:CONTrol[:ENAble]?", .callback = SCPI_LeptonOemPixelNoiseFilterControlQ,},
	{.pattern = "LEPton:OEM:PIXel:NOISE:FILter:CONTrol[:ENAble]", .callback = SCPI_LeptonOemPixelNoiseFilterControl,},

	{.pattern = "LEPton:VIDeo:PCOLOR:LUT?", .callback = SCPI_LeptonVideoPColorLutQ,},
	{.pattern = "LEPton:VIDeo:PCOLOR:LUT", .callback = SCPI_LeptonVideoPColorLut,},
	{.pattern = "LEPton:VIDeo:FOCus:CALculation[:ENAble:STATe]?", .callback = SCPI_LeptonVideoFocusCalculationEnableStateQ,},
	{.pattern = "LEPton:VIDeo:FOCus:CALculation[:ENAble:STATe]", .callback = SCPI_LeptonVideoFocusCalculationEnableState,},
	{.pattern = "LEPton:VIDeo:FOCus:ROI?", .callback = SCPI_LeptonVideoFocusRoiQ,},
	{.pattern = "LEPton:VIDeo:FOCus:ROI", .callback = SCPI_LeptonVideoFocusRoi,},
	{.pattern = "LEPton:VIDeo:FOCus:METRic:THREShold?", .callback = SCPI_LeptonVideoFocusMetricThresholdQ,},
	{.pattern = "LEPton:VIDeo:FOCus:METRic:THREShold", .callback = SCPI_LeptonVideoFocusMetricThreshold,},
	{.pattern = "LEPton:VIDeo:FOCus:METRic?", .callback = SCPI_LeptonVideoFocusMetricQ,},
	{.pattern = "LEPton:VIDeo:FREEZe:ENAble:STATe?", .callback = SCPI_LeptonVideoFreezeEnableStateQ,},
	{.pattern = "LEPton:VIDeo:FREEZe:ENAble:STATe", .callback = SCPI_LeptonVideoFreezeEnableState,},
	{.pattern = "LEPton:VIDeo:OUTput:FORMat?", .callback = SCPI_LeptonOemVideoOutputFormatQ,},
	{.pattern = "LEPton:VIDeo:OUTput:FORMat", .callback = SCPI_LeptonOemVideoOutputFormat,},


	//{.pattern = "LEPton:IMAGe:SNAP?", .callback = SCPI_LeptonImageSnapQ,},
	{.pattern = "LEPton:IMAGe:TIMEout?", .callback = SCPI_LeptonImageTimeoutQ,},
	{.pattern = "LEPton:IMAGe:TIMEout", .callback = SCPI_LeptonImageTimeout,},

	{.pattern = "TS", .callback = SCPI_TS,},

	SCPI_CMD_LIST_END
};

scpi_interface_t scpi_interface = {
    .error = SCPI_Error,
    .write = SCPI_Write,
    .control = SCPI_Control,
    .flush = SCPI_Flush,
    .reset = SCPI_Reset,
};

char scpi_input_buffer[SCPI_INPUT_BUFFER_LENGTH];
scpi_error_t scpi_error_queue_data[SCPI_ERROR_QUEUE_SIZE];

scpi_t scpi_context;
