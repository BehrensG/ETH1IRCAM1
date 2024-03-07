/*
 * Lepton.h
 *
 *  Created on: Feb 14, 2024
 *      Author: grzegorz
 */

#ifndef BSP_LEPTON_INC_LEPTON_H_
#define BSP_LEPTON_INC_LEPTON_H_

#include <stdbool.h>

#include "ip_addr.h"
#include "CCI.h"

#define LEP_WIDTH					160
#define LEP_HEIGHT					120
#define LEP_RAW14_PKT_LENGTH 		164
#define LEP_RGB888_PKT_LENGTH 		244
#define LEP_TEL_PACKETS 			3
#define LEP_RGB_PIXELS				3
#define LEP_SEGMENT					30
#define LEP_RGB888_PAYLOAD			240
#define LEP_RGB888_SIZE				(LEP_RGB_PIXELS*)
#define LEP_TEL_PKT_LEN				(LEP_RAW14_PKT_LENGTH - 4)
#define LEP_TEL_WORDS			  	(LEP_TEL_PACKETS * LEP_TEL_PKT_LEN / 2)

typedef struct
{

	char oem_part_number[32];
	uint8_t is_radiometric;
	uint64_t sys_serial_number;

}lep_info_t;

typedef struct
{
	ip_addr_t* addr;
	uint16_t port;
	uint32_t timeout;
	uint32_t tick;

}lep_udp_client_t;

typedef struct
{
	uint8_t agc_set_enabled;        // Set when agc_enabled
	int32_t emissivity;              // Integer percent 1 - 100
	int32_t gain_mode;               // SYS_GAIN_HIGH / SYS_GAIN_LOW / SYS_GAIN_AUTO
	uint16_t image_format;

}lep_config_t;


typedef struct
{

	lep_info_t info;
	lep_config_t config;
	lep_udp_client_t udp_client;

} lep_data_t;


typedef enum
{
	LEP_INIT_BOOT,
	LEP_INIT_REBOOT

}lep_init_state_e;


void LEP_CreateMainTask(void);
void LEP_CreateUDPTask(void);

cci_result_t LEP_Init(lep_init_state_e state, uint32_t timeout);

#endif /* BSP_LEPTON_INC_LEPTON_H_ */
