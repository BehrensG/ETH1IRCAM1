/*
 * BSP.c
 *
 *  Created on: Feb 16, 2024
 *      Author: grzegorz
 */

#include <string.h>
#include <stdbool.h>

#include "BSP.h"
#include "SCPI_Def.h"
#include "EEPROM.h"

bsp_t bsp;
bsp_eeprom_t eeprom_default;

static void BSP_Init_Common()
{

	bsp.security.status = true;
	bsp.default_cfg = false;
	bsp.temperature_unit = KELVIN;


}

void BSP_Init_DefualtEEPROM()
{
	bsp.eeprom.structure.ip4_static.MAC[0] = MAC_0;
	bsp.eeprom.structure.ip4_static.MAC[1] = MAC_1;
	bsp.eeprom.structure.ip4_static.MAC[2] = MAC_2;
	bsp.eeprom.structure.ip4_static.MAC[3] = MAC_3;
	bsp.eeprom.structure.ip4_static.MAC[4] = MAC_4;
	bsp.eeprom.structure.ip4_static.MAC[5] = MAC_5;

	bsp.eeprom.structure.ip4_static.gateway[0] = GATEWAY_ADDRESS_0;
	bsp.eeprom.structure.ip4_static.gateway[1] = GATEWAY_ADDRESS_1;
	bsp.eeprom.structure.ip4_static.gateway[2] = GATEWAY_ADDRESS_2;
	bsp.eeprom.structure.ip4_static.gateway[3] = GATEWAY_ADDRESS_3;

	bsp.eeprom.structure.ip4_static.ip[0] = IP_ADDRESS_0;
	bsp.eeprom.structure.ip4_static.ip[1] = IP_ADDRESS_1;
	bsp.eeprom.structure.ip4_static.ip[2] = IP_ADDRESS_2;
	bsp.eeprom.structure.ip4_static.ip[3] = IP_ADDRESS_3;

	bsp.eeprom.structure.ip4_static.netmask[0] = NETMASK_ADDRESS_0;
	bsp.eeprom.structure.ip4_static.netmask[1] = NETMASK_ADDRESS_1;
	bsp.eeprom.structure.ip4_static.netmask[2] = NETMASK_ADDRESS_2;
	bsp.eeprom.structure.ip4_static.netmask[3] = NETMASK_ADDRESS_3;

	bsp.eeprom.structure.ip4_static.tcp_port = 2000;
	bsp.eeprom.structure.ip4_static.udp_port = 16384;

	strncpy(bsp.eeprom.structure.service.password, PASSWORD, STRING_LENGTH);
	strncpy(bsp.eeprom.structure.info.serial_number, SCPI_IDN4, STRING_LENGTH);
	strncpy(bsp.eeprom.structure.info.software_version, SCPI_IDN3, STRING_LENGTH);

	eeprom_default = bsp.eeprom;

}

static void BSP_Init_IP4Current()
{
	bsp.ip4_current.MAC[0] = bsp.eeprom.structure.ip4_static.MAC[0];
	bsp.ip4_current.MAC[1] = bsp.eeprom.structure.ip4_static.MAC[1];
	bsp.ip4_current.MAC[2] = bsp.eeprom.structure.ip4_static.MAC[2];
	bsp.ip4_current.MAC[3] = bsp.eeprom.structure.ip4_static.MAC[3];
	bsp.ip4_current.MAC[4] = bsp.eeprom.structure.ip4_static.MAC[4];
	bsp.ip4_current.MAC[5] = bsp.eeprom.structure.ip4_static.MAC[5];

	bsp.ip4_current.gateway[0] = bsp.eeprom.structure.ip4_static.gateway[0];
	bsp.ip4_current.gateway[1] = bsp.eeprom.structure.ip4_static.gateway[1];
	bsp.ip4_current.gateway[2] = bsp.eeprom.structure.ip4_static.gateway[2];
	bsp.ip4_current.gateway[3] = bsp.eeprom.structure.ip4_static.gateway[3];

	bsp.ip4_current.ip[0] = bsp.eeprom.structure.ip4_static.ip[0];
	bsp.ip4_current.ip[1] = bsp.eeprom.structure.ip4_static.ip[1];
	bsp.ip4_current.ip[2] = bsp.eeprom.structure.ip4_static.ip[2];
	bsp.ip4_current.ip[3] = bsp.eeprom.structure.ip4_static.ip[3];

	bsp.ip4_current.netmask[0] = bsp.eeprom.structure.ip4_static.netmask[0];
	bsp.ip4_current.netmask[1] = bsp.eeprom.structure.ip4_static.netmask[1];
	bsp.ip4_current.netmask[2] = bsp.eeprom.structure.ip4_static.netmask[2];
	bsp.ip4_current.netmask[3] = bsp.eeprom.structure.ip4_static.netmask[3];

	bsp.ip4_current.tcp_port = bsp.eeprom.structure.ip4_static.tcp_port;
	bsp.ip4_current.udp_port = bsp.eeprom.structure.ip4_static.udp_port;
}

bsp_result_t BSP_Init()
{
	bsp_result_t status;

	BSP_Init_Common();
	BSP_Init_DefualtEEPROM();

	if(!(LL_GPIO_ReadInputPort(MCU_DEFAULT_GPIO_Port) & MCU_DEFAULT_Pin))
	{
		BSP_Init_DefualtEEPROM();
		BSP_Init_IP4Current();
		bsp.default_cfg = true;
	}
	else
	{
		status = EEPROM_Status();

		switch (status)
		{
			case BSP_EEPROM_EMPTY:
			{
				BSP_Init_DefualtEEPROM();
				status = EEPROM_Write(&bsp.eeprom, EEPROM_CFG_SIZE);
				status = EEPROM_Read(&bsp.eeprom, EEPROM_CFG_SIZE);
				if(BSP_OK == status)
				{
					BSP_Init_IP4Current();
					bsp.default_cfg = false;
				}; break;
			}
			case BSP_OK:
			{
				status = EEPROM_Read(&bsp.eeprom, EEPROM_CFG_SIZE);
				if(BSP_OK == status)
				{
					BSP_Init_IP4Current();
					bsp.default_cfg = false;
				}; break;
			}
			default:
			{
				BSP_Init_DefualtEEPROM();
				BSP_Init_IP4Current();
				bsp.default_cfg = true;
			}; break;
		}

	}

	return status;
}
