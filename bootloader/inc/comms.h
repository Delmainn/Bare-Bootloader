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



#endif // INC_COMMS_H