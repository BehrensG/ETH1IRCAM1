/*
 * bsp.h
 *
 *  Created on: Feb 10, 2023
 *      Author: grzegorz
 */

#ifndef THIRD_PARTY_BSP_INC_BSP_H_
#define THIRD_PARTY_BSP_INC_BSP_H_

#include <stdbool.h>

#include "main.h"

/*************************************** ETHERNET ***************************************/

#define IP_ADDRESS_0 192
#define IP_ADDRESS_1 168
#define IP_ADDRESS_2 1
#define IP_ADDRESS_3 56

#define NETMASK_ADDRESS_0 255
#define NETMASK_ADDRESS_1 255
#define NETMASK_ADDRESS_2 255
#define NETMASK_ADDRESS_3 0

#define GATEWAY_ADDRESS_0 192
#define GATEWAY_ADDRESS_1 168
#define GATEWAY_ADDRESS_2 1
#define GATEWAY_ADDRESS_3 1

#define MAC_0 0x00
#define MAC_1 0x80
#define MAC_2 0xE1
#define MAC_3 0x00
#define MAC_4 0x00
#define MAC_5 0x00

#define ADC_SAMPLE_SIZE	10000
#define LIST_SIZE		10000


#define STRING_LENGTH 		16

#define PASSWORD "ETH1IRCAM1"

#define TCPIP_DEFAULT_PORT 2000

#define SELECT_ADC1		1
#define SELECT_ADC2		2

typedef enum bsp_result
{
	BSP_OK = 0,
	BSP_COMM_OK = BSP_OK,
	BSP_ERROR = -1,
	BSP_ERROR_TIMEOUT = -2,

	/* I2C Errors */
	BSP_ERROR_I2C_BUS_NOT_READY = -20,
	BSP_ERROR_I2C_BUFFER_OVERFLOW = -22,
	BSP_ERROR_I2C_ARBITRATION_LOST = -23,
	BSP_ERROR_I2C_BUS_ERROR = -24,
	BSP_ERROR_I2C_NACK_RECEIVED = -25,
	BSP_ERROR_I2C_FAIL = -26,

	BSP_EEPROM_WRITE_ERROR = -50,
	BSP_EEPROM_READ_ERROR = -51,
	BSP_EEPROM_NO_CONNECTION = -52,
	BSP_EEPROM_EMPTY = -53

} bsp_result_t;

enum format_data_enum
{
	DATA_FORMAT_ASCII = 0,
	DATA_FORMAT_REAL = 1
};

#pragma pack(push, 1)


typedef struct
{

	char serial_number[STRING_LENGTH];
	char software_version[STRING_LENGTH];

}bsp_scpi_info_t;


typedef struct
{
	uint8_t ip[4];
	uint8_t netmask[4];
	uint8_t gateway[4];
	uint8_t MAC[6];
	uint16_t tcp_port;
	uint16_t udp_port;

}bsp_ip4_lan_t;



typedef struct
{
	char password[STRING_LENGTH];

}bsp_service_t;

#define EEPROM_CFG_SIZE 	sizeof(bsp_ip4_lan_t) + sizeof(bsp_scpi_info_t) + sizeof(bsp_service_t)

typedef union
{
	struct data
	{

		bsp_ip4_lan_t ip4_static;
		bsp_scpi_info_t info;
		bsp_service_t service;

	}structure;
	uint8_t bytes[EEPROM_CFG_SIZE];

}bsp_eeprom_t;

#pragma pack(pop)


typedef struct
{
	bool status;

}bsp_security_t;

typedef enum
{
	CELSIUS = 1,
	FAHRENHEIT,
	KELVIN
}bsp_temperature_t;


typedef struct
{
	bsp_eeprom_t eeprom;
	bsp_security_t security;
	bsp_ip4_lan_t ip4_current;
	bool default_cfg;
	bsp_temperature_t temperature_unit;

}bsp_t;

extern bsp_t bsp;

bsp_result_t BSP_Init();

#endif /* THIRD_PARTY_BSP_INC_BSP_H_ */
