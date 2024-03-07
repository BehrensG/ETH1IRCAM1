/*
 * VoSPI.h
 *
 *  Created on: Feb 15, 2024
 *      Author: grzegorz
 */

#ifndef BSP_LEPTON_INC_VOSPI_H_
#define BSP_LEPTON_INC_VOSPI_H_


#include "main.h"
#include "stdbool.h"

// The size of a single VoSPI packet
#define VOSPI_RAW14_BYTES 164
#define VOSPI_RAW14_SYMBOLS 160
#define VOSPI_RGB888_BYTES 244
#define VOSPI_RGB888_SYMBOLS 240

// The maximum number of packets per segment, sufficient to include telemetry
#define VOSPI_MAX_PACKETS_PER_SEGMENT 61
// The number of packets in segments with and without telemetry lines present
#define VOSPI_PACKETS_PER_SEGMENT_NORMAL 60
#define VOSPI_PACKETS_PER_SEGMENT_TELEMETRY 61

// The number of segments per frame
#define VOSPI_SEGMENTS_PER_FRAME 4

// The maximum number of resets allowed before giving up on synchronising
#define VOSPI_MAX_SYNC_RESETS 20
// The maximum number of invalid frames before giving up and assuming we've lost sync
// FFC duration is nominally 23 frames, so we should never exceed that
#define VOSPI_MAX_INVALID_FRAMES 25

// A single VoSPI packet
typedef struct {
  uint16_t id;
  uint16_t crc;
  uint8_t raw14[VOSPI_RAW14_SYMBOLS];
  uint8_t rgb888[VOSPI_RGB888_SYMBOLS];

} vospi_packet_t;

// A single VoSPI segment
typedef struct {
  vospi_packet_t packets[VOSPI_MAX_PACKETS_PER_SEGMENT];
  uint16_t packet_count;

} vospi_segment_t;

// A single VoSPI frame
typedef struct {
  vospi_segment_t segments[VOSPI_SEGMENTS_PER_FRAME];
  bool valid;
} vospi_frame_t;


bool VoSPI_TransferFrame();
bool VoSPI_SyncAndTransferFrame();

#endif /* BSP_LEPTON_INC_VOSPI_H_ */
