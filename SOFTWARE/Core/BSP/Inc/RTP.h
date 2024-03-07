/*
 * RTP.h
 *
 *  Created on: Feb 18, 2024
 *      Author: grzegorz
 */

#ifndef BSP_INC_RTP_H_
#define BSP_INC_RTP_H_

#define RTP_UDP_PORT 16384

err_t RTP_SendPacket(struct netconn* conn, uint8_t lep_segment, uint16_t rtp_seq_num, uint32_t rtp_ssrc, uint32_t rtp_ext_hdr1);

#endif /* BSP_INC_RTP_H_ */
