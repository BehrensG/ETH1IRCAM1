/*
 * VoSPI.c
 *
 *  Created on: Feb 15, 2024
 *      Author: grzegorz
 *
 *      Code base on https://github.com/themainframe/leptonic/tree/master
 */

#include <stdbool.h>
#include <string.h>

#include "cmsis_os.h"
#include "VoSPI.h"
#include "Lepton.h"
#include "CCI_OEM.h"
#include "utility.h"

extern SPI_HandleTypeDef hspi1;
extern lep_data_t lepton;

#define VOSIP_INVALID_PKG 		0x0F
#define VOSIP_PKG_HEADER_SIZE	4
#define VOSIP_MAX_PKT_LENGTH	244


vospi_frame_t frame;

bool VoSPI_TransferSegment(uint8_t seg)
{
	uint8_t spi_data[VOSPI_RGB888_BYTES];
	uint32_t size;
	uint32_t start = HAL_GetTick();

	frame.valid = false;

	(CCI_VIDEO_OUTPUT_FORMAT_RAW14 == lepton.config.image_format) ? (size = VOSPI_RAW14_BYTES) : (size = VOSPI_RGB888_BYTES);

	while (!LL_GPIO_IsInputPinSet(LEP_VSYNC_GPIO_Port, LEP_VSYNC_Pin))
	{
	};

	// Perform the spidev transfer

	if (HAL_OK != HAL_SPI_Receive(&hspi1, spi_data, size, 50))
	{

		return false;
	}

	frame.segments[seg].packets[0].id = spi_data[0] << 8 | spi_data[1];
	frame.segments[seg].packets[0].crc = spi_data[2] << 8 | spi_data[3];

	// Flip the byte order of the ID & CRC

	while ((frame.segments[seg].packets[0].id & 0x0f00) == 0x0f00)
	{
		// It was a discard packet, try receiving another packet into the same buf

		HAL_SPI_Receive(&hspi1, spi_data, size, 50);

		frame.segments[seg].packets[0].id = spi_data[0] << 8 | spi_data[1];
		frame.segments[seg].packets[0].crc = spi_data[2] << 8 | spi_data[3];

		if(!UTIL_Timeout(start, 100000))
			return false;

	}

	if (CCI_VIDEO_OUTPUT_FORMAT_RAW14 == lepton.config.image_format)
	{
		memcpy(frame.segments[seg].packets[0].raw14, spi_data + 4, VOSPI_RAW14_SYMBOLS);
	}
	else
	{
		memcpy(frame.segments[seg].packets[0].rgb888, spi_data + 4, VOSPI_RGB888_SYMBOLS);
	}


	for (uint8_t x = 1; x < 60; x++)
	{
		if (HAL_OK != HAL_SPI_Receive(&hspi1, spi_data, size, 50))
		{

			return false;
		}

		if (CCI_VIDEO_OUTPUT_FORMAT_RAW14 == lepton.config.image_format)
		{
			memcpy(frame.segments[seg].packets[x].raw14, spi_data + 4, VOSPI_RAW14_SYMBOLS);
		}
		else
		{
			memcpy(frame.segments[seg].packets[x].rgb888, spi_data + 4, VOSPI_RGB888_SYMBOLS);
		}

		frame.segments[seg].packets[x].id = spi_data[0] << 8 | spi_data[1];
		frame.segments[seg].packets[x].crc = spi_data[2] << 8 | spi_data[3];
	}

	frame.valid = true;
	return true;
}

/**
 * Synchroise the VoSPI stream and transfer a single frame.
 * Returns the number of successfully-transferred frames (0 or 1).
 */
bool VoSPI_SyncAndTransferFrame()
{

	uint16_t packet_20_num;
	uint8_t ttt_bits, resets = 0;

	while (1)
	{

		// Stream a first segment
		if (!VoSPI_TransferSegment(0))
		{
			return false;
		}

		// If the packet number isn't even correct, we'll reset the bus to sync
		packet_20_num = frame.segments[0].packets[20].id & 0xff;

		if (packet_20_num != 20)
		{
			// Deselect the chip, wait 200ms with CS deasserted
			osDelay(pdMS_TO_TICKS(200));

			if (++resets >= VOSPI_MAX_SYNC_RESETS)
			{
				return false;
			}

			continue;
		}

		// Check we're looking at the first segment, if not, just keep reading until we get there
		ttt_bits = frame.segments[0].packets[20].id >> 12;
		if (ttt_bits == 1)
		{
			break;
		}
	}

	// Receive the remaining segments
	for (uint8_t seg = 1; seg < VOSPI_SEGMENTS_PER_FRAME; seg++)
	{
		VoSPI_TransferSegment(seg);
	}

	return true;
}

/**
 * Transfer a frame.
 * Assumes that we're already synchronised with the VoSPI stream.
 */
bool VoSPI_TransferFrame()
{
	uint8_t ttt_bits, restarts = 0;
	frame.valid = false;
	// Receive all segments
	for (uint8_t seg = 0; seg < VOSPI_SEGMENTS_PER_FRAME; seg++)
	{
		VoSPI_TransferSegment(seg);

		ttt_bits = frame.segments[seg].packets[20].id >> 12;
		if (ttt_bits != seg + 1)
		{
			seg--;
			if (restarts++ > VOSPI_MAX_INVALID_FRAMES * 4)
			{
				return false;
			}
			continue;
		}
	}

	return true;
}
