/*
 * RTC.c
 *
 *  Created on: Feb 18, 2024
 *      Author: grzegorz
 */

#include <string.h>

#include "api.h"
#include "BSP.h"
#include "VoSPI.h"
#include "Lepton.h"
#include "CCI_OEM.h"
#include "utility.h"

extern lep_data_t lepton;
extern vospi_frame_t frame;

#define RTP_PAYLOAD_TYPE	0x7F // Example payload type (dynamic range)
#define RTP_VERSION  		0x80
#define RTP_EXTENSION		0x10

// RTP header structure
typedef struct {
	  PACK_STRUCT_FLD_8(uint8_t  version);
	  PACK_STRUCT_FLD_8(uint8_t  payload_type);
	  PACK_STRUCT_FIELD(uint16_t sequence_number);
	  PACK_STRUCT_FIELD(uint32_t timestamp);
	  PACK_STRUCT_FIELD(uint32_t ssrc);
	  PACK_STRUCT_FIELD(uint32_t csrc);
	  PACK_STRUCT_FIELD(uint32_t extension_header_id);
	  PACK_STRUCT_FIELD(uint32_t extension_header1);
	  PACK_STRUCT_FIELD(uint32_t extension_header2);

} rtp_header_t;


static uint8_t* RTP_FlattenSegment(uint8_t seg, size_t* size_readout, uint32_t* crc)
{
	uint32_t offset = 0;
	uint8_t *arr_1d;
	uint8_t package_number = VOSPI_PACKETS_PER_SEGMENT_NORMAL;

	if(CCI_VIDEO_OUTPUT_FORMAT_RAW14 == lepton.config.image_format)
	{
		*size_readout = VOSPI_PACKETS_PER_SEGMENT_NORMAL * VOSPI_RAW14_SYMBOLS;
		arr_1d = (uint8_t*)malloc(VOSPI_PACKETS_PER_SEGMENT_NORMAL * VOSPI_RAW14_SYMBOLS);
	}
	else
	{
		*size_readout = VOSPI_PACKETS_PER_SEGMENT_NORMAL * VOSPI_RGB888_SYMBOLS;
		arr_1d = (uint8_t*)malloc(VOSPI_PACKETS_PER_SEGMENT_NORMAL * VOSPI_RGB888_SYMBOLS);
	}

	for (uint16_t i = 0; i < package_number; i++)
	{

		if(CCI_VIDEO_OUTPUT_FORMAT_RAW14 ==lepton.config.image_format)
		{
			offset = i * VOSPI_RAW14_SYMBOLS;
			memcpy((arr_1d + offset), frame.segments[seg].packets[i].raw14, VOSPI_RAW14_SYMBOLS);

		}
		else
		{
			offset = i * VOSPI_RGB888_SYMBOLS;
			memcpy((arr_1d + offset), frame.segments[seg].packets[i].rgb888, VOSPI_RGB888_SYMBOLS);
		}
	}

	//*crc = UTIL_CalculateCRC(arr_1d, *size_readout);
	*crc = 0;
	return arr_1d;
}


err_t RTP_SendPacket(struct netconn* conn, uint8_t lep_segment, uint16_t rtp_seq_num, uint32_t rtp_ssrc, uint32_t rtp_ext_hdr1)
{
    struct netbuf* buf;
    rtp_header_t* rtp_header;
    err_t err = ERR_OK;
    size_t size;
    uint32_t crc;

    uint8_t* data = RTP_FlattenSegment(lep_segment, &size, &crc);

    // Allocate memory for the RTP packet
    buf = netbuf_new();
    if (buf != NULL) {
        // Reserve space for the RTP header
        void *payload = netbuf_alloc(buf, sizeof(rtp_header_t) + size);

        if (payload != NULL) {
        	// Construct the RTP header
            rtp_header = (rtp_header_t *)payload;
            rtp_header->version = RTP_VERSION | RTP_EXTENSION;
            rtp_header->payload_type = (uint8_t)lepton.config.image_format & 0x7F;
            rtp_header->sequence_number = htons(rtp_seq_num);
            rtp_header->timestamp = htonl(HAL_GetTick());
            rtp_header->ssrc = htonl(rtp_ssrc); // Example SSRC
            rtp_header->csrc = 0x00000000;
            rtp_header->extension_header_id = 2;
            rtp_header->extension_header1 = rtp_ext_hdr1;
            rtp_header->extension_header2 = crc;
            // Copy the audio data into the packet
            memcpy(payload + sizeof(rtp_header_t), data, size);

            // Connect the netconn to the destination address and port
          //  netconn_connect(conn, dest_addr, dest_port);

            // Send the RTP packet
            err = netconn_send(conn, buf);

            if (err != ERR_OK) {
                // Handle send error
            	netbuf_delete(buf);
            	free(data);
            }

        } else {
            // Handle memory allocation error for payload
        }

        // Free the netbuf
        netbuf_delete(buf);
        free(data);

    } else {
        // Handle netbuf allocation error
        netbuf_delete(buf);
        free(data);
    }

    return err;
}
