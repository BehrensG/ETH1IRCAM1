/*
 * Lepton.c
 *
 *  Created on: Feb 14, 2024
 *      Author: grzegorz
 */

#include <string.h>
#include <stdio.h>

#include "cmsis_os.h"
#include "err.h"
#include "ip_addr.h"
#include "udp.h"
#include "api.h"

#include "LED.h"
#include "RTP.h"
#include "VoSPI.h"
#include "CCI_OEM.h"
#include "CCI_SYS.h"
#include "utility.h"
#include "Lepton.h"

extern uint8_t IP_ADDRESS[4];
extern osMutexId_t LeptonMutexHandle;
extern vospi_frame_t frame;

lep_data_t lepton;

osThreadId_t LeptonMainTaskHandle;
uint32_t LeptonMainTaskBuffer[2048];
StaticTask_t LeptonMainTaskControlBlock;
const osThreadAttr_t LeptonMainTask_attributes =
{ .name = "LeptonMainTask", .cb_mem = &LeptonMainTaskControlBlock, .cb_size =
		sizeof(LeptonMainTaskControlBlock), .stack_mem =
		&LeptonMainTaskBuffer[0], .stack_size = sizeof(LeptonMainTaskBuffer),
		.priority = (osPriority_t) osPriorityNormal7, };

osThreadId_t LeptonUDPTaskHandle;
uint32_t LeptonUDPTaskBuffer[2048];
StaticTask_t LeptonUDPTaskControlBlock;
const osThreadAttr_t LeptonUDPTask_attributes =
{ .name = "LeptonUDPTask", .cb_mem = &LeptonUDPTaskControlBlock, .cb_size =
		sizeof(LeptonUDPTaskControlBlock), .stack_mem = &LeptonUDPTaskBuffer[0],
		.stack_size = sizeof(LeptonUDPTaskBuffer), .priority =
				(osPriority_t) osPriorityBelowNormal6, };

static void StartLeptonMainTask(void *argument);
static void StartLeptonUDPTask(void *argument);
static void LEP_InitData(void);

void LEP_CreateMainTask(void)
{
	LeptonMainTaskHandle = osThreadNew(StartLeptonMainTask, NULL,
			&LeptonMainTask_attributes);
}

void LEP_CreateUDPTask(void)
{
	LeptonUDPTaskHandle = osThreadNew(StartLeptonUDPTask, NULL,
			&LeptonUDPTask_attributes);
}

cci_result_t LEP_Init(lep_init_state_e state, uint32_t timeout)
{
	uint32_t start = HAL_GetTick();
	cci_result_t status = CCI_OK;
	uint8_t boot_status = 0x00;

	LL_GPIO_ResetOutputPin(LEP_nRST_GPIO_Port, LEP_nRST_Pin);
	osDelay(pdMS_TO_TICKS(1000));

	while (CCI_BOOT_OK != boot_status)
	{
		status = CCI_ReadBootStatus(&boot_status);

		if (CCI_OK != status)
			break;

		if (!UTIL_Timeout(start, timeout))
		{
			status = CCI_NOT_READY;
			break;
		}
	}

	if (LEP_INIT_BOOT == state)
	{
		status = CCI_SetOemGpioMode(CCI_OEM_GPIO_MODE_VSYNC);
		status = CCI_GetOemFlirPartNumber(lepton.info.oem_part_number);
		status = CCI_SysFlirSerialNumber(&lepton.info.sys_serial_number);
	}


	return status;
}

static void LEP_InitData(void)
{
	lepton.config.image_format = CCI_VIDEO_OUTPUT_FORMAT_RAW14;

	lepton.udp_client.port = RTP_UDP_PORT;
	lepton.udp_client.timeout = 10000;
	lepton.udp_client.tick = HAL_GetTick();

	for (uint8_t seg = 0; seg < VOSPI_SEGMENTS_PER_FRAME; seg++)
	{
		frame.segments[seg].packet_count = VOSPI_PACKETS_PER_SEGMENT_NORMAL;
	}

	frame.valid = false;

}

static void LEP_GetFrame(bool *valid_frame, uint32_t timeout)
{
	bool loop = false;
	uint32_t timeout_start = HAL_GetTick();

	while (!loop)
	{
		if (*valid_frame)
		{
			*valid_frame = VoSPI_TransferFrame();
			loop = *valid_frame;
		}
		else
		{
			*valid_frame = VoSPI_SyncAndTransferFrame();
			loop = *valid_frame;
		}

		if (!UTIL_Timeout(timeout_start, 5000))
			break;
	}
}

cci_result_t lep_init_status = CCI_NOT_READY;

static void StartLeptonMainTask(void *argument)
{
	bool valid_frame = false;

	LEP_InitData();

	lep_init_status = LEP_Init(LEP_INIT_BOOT, 10000);

	if (CCI_OK ==lep_init_status)
		LEP_CreateUDPTask();

	LED_Control(BLUE, 0);

	for (;;)
	{


		if (CCI_OK == lep_init_status)
		{
			if (osOK == osMutexAcquire(LeptonMutexHandle, 10000))
			{
				LEP_GetFrame(&valid_frame, 5000);

				if (valid_frame)
					osMutexRelease(LeptonMutexHandle);

				osDelay(pdMS_TO_TICKS(10));

			}
			else
			{
				osDelay(pdMS_TO_TICKS(10));
			}
		}
		else
		{
			osDelay(pdMS_TO_TICKS(10));
		}

	}
}

/* --------------------------------------------------------------------------------------------------------------------
 *
 * Original code
 *
 * --------------------------------------------------------------------------------------------------------------------
 */

static struct netconn *conn;

static const char start[] = "start";
static const char stop[] = "stop";
static const char ack[] = "ack";


typedef enum
{
	UDP_STATE_OK = 0,
	UDP_STATE_ERROR,
	UDP_STATE_CREATE,
	UDP_STATE_LISTEN,
	UDP_STATE_STREAM,
	UDP_STATE_START,
	UDP_STATE_STOP,
	UDP_STATE_ACK

} udp_task_state_t;

uint16_t rtp_sequence_number = 0;

volatile bool rcv_new_data = false;

void LEP_NetconnCallback(struct netconn* conn, enum netconn_evt even, u16_t len)
{
	if(NETCONN_EVT_RCVPLUS == even)
	{
		if (0 != len)
			rcv_new_data = true;
	}

}

static udp_task_state_t UDP_Create()
{
	err_t status;
	conn = netconn_new_with_callback(NETCONN_UDP, LEP_NetconnCallback);
	status = netconn_bind(conn, IP_ADDR_ANY, RTP_UDP_PORT);
	netconn_set_recvtimeout(conn, 5000);

	if (ERR_OK != status)
	{
		return UDP_STATE_ERROR;
	}

	return UDP_STATE_START;

}


static udp_task_state_t UDP_Start()
{
	struct netbuf* buf;

	void* data;
	uint16_t len;

	err_t status;
	udp_task_state_t state;

	if(rcv_new_data)
	{
		status = netconn_recv(conn, &buf);

		if (ERR_OK != status)
		{
			netbuf_delete(buf);
			state = UDP_STATE_START;

		}
		else
		{

			lepton.udp_client.addr = netbuf_fromaddr(buf);  // get the address of the client
			lepton.udp_client.port = netbuf_fromport(buf);  // get the Port of the client

			netbuf_data(buf, &data, &len);

			if(!strncmp(data, start, strlen(start)))
			{
				status = netconn_connect(conn, lepton.udp_client.addr, lepton.udp_client.port);
				state = UDP_STATE_STREAM;
				lepton.udp_client.tick = HAL_GetTick();

			}
			else
			{
				state = UDP_STATE_START;
			}

			netbuf_delete(buf);
		}

		rcv_new_data = false;
	}
	else
	{
		state = UDP_STATE_START;
	}

	return state;
}


static udp_task_state_t UDP_Send(uint32_t rtp_ssrc)
{
	err_t err;
	volatile udp_task_state_t state = UDP_STATE_STREAM;

	if (osOK == osMutexAcquire(LeptonMutexHandle, 10000))
	{
		for (uint8_t x = 0; x < VOSPI_SEGMENTS_PER_FRAME; x++)
		{
			err = RTP_SendPacket(conn, x, rtp_sequence_number, rtp_ssrc, (x+1));
			osDelay(pdMS_TO_TICKS(1));

			if (ERR_OK != err)
				break;
		}

		rtp_sequence_number++;

		if (ERR_OK != err)
		{
			osMutexRelease(LeptonMutexHandle);
			state = UDP_STATE_STREAM;

		}
		else
		{
			if(UTIL_Timeout(lepton.udp_client.tick, lepton.udp_client.timeout))
			{
				state = UDP_STATE_STREAM;
			}
			else
			{
				state = UDP_STATE_STOP;
			}

			osMutexRelease(LeptonMutexHandle);
		}
	}
	else
	{
		//osDelay(pdMS_TO_TICKS(1));
		state = UDP_STATE_STREAM;
	}

	return state;
}

static udp_task_state_t UDP_Receive()
{
	err_t status;
	struct netbuf* buf;
	udp_task_state_t state;
	void* data;
	uint16_t len;

	status = netconn_recv(conn, &buf);

	if (ERR_OK != status)
	{
		netbuf_delete(buf);
		state = UDP_STATE_START;
	}
	else
	{
		netbuf_data(buf, &data, &len);
		netbuf_delete(buf);

		if (!strncmp(data, ack, strlen(ack)))
		{
			state = UDP_STATE_ACK;
		}
		else if (!strncmp(data, stop, strlen(stop)))
		{
			state = UDP_STATE_STOP;
		}
		else
		{
			state = UDP_STATE_STREAM;
		}

	}

	return state;
}

static udp_task_state_t UDP_Stream(uint32_t rtp_ssrc)
{
	udp_task_state_t state = UDP_STATE_STREAM;

	if(!rcv_new_data)
	{
		if (frame.valid)
			state = UDP_Send(rtp_ssrc);
	}
	else
	{
		state = UDP_Receive();
	}

	return state;
}

static udp_task_state_t UDP_Stop()
{

	rcv_new_data = false;

	netconn_close(conn);
	netconn_delete(conn);

	return UDP_STATE_CREATE;
}

static udp_task_state_t UDP_Ack()
{
	rcv_new_data = false;
	lepton.udp_client.tick = HAL_GetTick();
	return UDP_STATE_STREAM;
}

static void StartLeptonUDPTask(void *argument)
{
	udp_task_state_t state = UDP_STATE_CREATE;

	uint32_t rtp_ssrc = 0;

	rtp_ssrc = UTIL_RandomNumbersGeneration();

	for (;;)
	{
		switch (state)
		{
			case UDP_STATE_CREATE: state = UDP_Create(); break;
			case UDP_STATE_START: state = UDP_Start(); break;
			case UDP_STATE_STREAM: state = UDP_Stream(rtp_ssrc); break;
			case UDP_STATE_STOP: state = UDP_Stop(); break;
			case UDP_STATE_ACK: state = UDP_Ack(); break;
			default: state = UDP_STATE_STREAM; break;
		}
		osDelay(pdMS_TO_TICKS(5));
	}
}
