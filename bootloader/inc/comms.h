#ifndef INC_COMMS_H
#define INC_COMMS_H

#include "common-defines.h"
// A packet is: 1 length byte + 16 data bytes + 1 CRC byte.
// Data shorter than 16 bytes is padded with 0xff, and the padding
// IS included in the CRC calculation 

#define PACKET_DATA_LENGTH   (16)
#define PACKET_LENGTH_BYTES  (1)
#define PACKET_CRC_BYTES     (1)
#define PACKET_LENGTH        (PACKET_LENGTH_BYTES + PACKET_DATA_LENGTH + PACKET_CRC_BYTES)

// Reserved single-byte packet payloads (data[0]), used for control
// messages rather than actual firmware data.
#define PACKET_RETX_DATA0    (0x19)
#define PACKET_ACK_DATA0     (0x15)

#define BL_PACKET_SYNC_OBSERVED_DATA0      (0x20)
#define BL_PACKET_FW_UPDATE_REQ_DATA0      (0x31)
#define BL_PACKET_FW_UPDATE_RES_DATA0      (0x37)
#define BL_PACKET_DEVICE_ID_REQ_DATA0      (0x3C)
#define BL_PACKET_DEVICE_ID_RES_DATA0      (0x3F)
#define BL_PACKET_FW_LENGTH_REQ_DATA0      (0x42)
#define BL_PACKET_FW_LENGTH_RES_DATA0      (0X45)
#define BL_PACKET_READY_FOR_DATA_DATA0     (0X48)
#define BL_PACKET_UPDATE_SUCCESSFUL_DATA0  (0X54)
#define BL_PACKET_NACK_DATA0               (0X59)


typedef struct comms_packet_t {
    uint8_t length;
    uint8_t data[PACKET_DATA_LENGTH];
    uint8_t crc;
} comms_packet_t;

void comms_setup(void);
void comms_update(void);


bool comms_packets_available(void);
void comms_write(comms_packet_t* packet);
void comms_read(comms_packet_t* packet);
uint8_t comms_compute_crc(comms_packet_t* packet);
bool comms_is_single_byte_packet(const comms_packet_t* packet, uint8_t byte);
void comms_create_single_byte_packet(comms_packet_t* packet, uint8_t byte);



#endif // INC_COMMS_H