/*
 * Telemetry.c
 *
 *  Created on: Feb 26, 2024
 *      Author: BehrensG
 */

// Code base on https://github.com/themainframe/leptonic

#include "Telemetry.h"

telemetry_data_t parse_telemetry_packet(vospi_packet_t* packet)
{
  uint32_t reordered_status_bits = LEPTON_DWORD(packet->raw14, 6);

  // Decode the status bits first
  telemetry_data_status_bits_t status_bits = {
    .ffc_desired = (reordered_status_bits & 0x08) >> 3,
    .ffc_state = (reordered_status_bits & 0x30) >> 4,
    .agc_state = (reordered_status_bits & 0x1000) >> 12,
    .shutter_lockout = (reordered_status_bits & 0x4000) >> 15,
    .overtemp_shutdown_imminent = (reordered_status_bits & 0x100000) >> 20
  };

  telemetry_data_t telemetry_data = {
    .revision = LEPTON_WORD(packet->raw14, 0),
    .msec_since_boot = LEPTON_DWORD(packet->raw14, 2),
    .status_bits = status_bits,
    .frame_count = LEPTON_DWORD(packet->raw14, 40),
    .frame_mean = LEPTON_WORD(packet->raw14, 44),
    .fpa_temp_count = LEPTON_WORD(packet->raw14, 46),
    .fpa_temp_kelvin_100 = LEPTON_WORD(packet->raw14, 48),
    .fpa_temp_last_ffc_kelvin_100 = LEPTON_WORD(packet->raw14, 58),
    .msec_last_ffc = LEPTON_DWORD(packet->raw14, 60),
    .agc_roi_top = LEPTON_WORD(packet->raw14, 68),
    .agc_roi_left = LEPTON_WORD(packet->raw14, 70),
    .agc_roi_bottom = LEPTON_WORD(packet->raw14, 72),
    .agc_roi_right = LEPTON_WORD(packet->raw14, 74),
    .agc_clip_limit_high = LEPTON_WORD(packet->raw14, 76),
    .agc_clip_limit_low = LEPTON_WORD(packet->raw14, 78),
    .video_output_format = LEPTON_DWORD(packet->raw14, 144)
  };

  return telemetry_data;
}
