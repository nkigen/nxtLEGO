#ifndef __CLIENT_REQ_H__
#define __CLIENT_REQ_H__
#include "../../common/bt_packet.h"


void bt_decode_incoming(bt_packet_t *incoming, bt_packet_t *outgoing);
#endif
