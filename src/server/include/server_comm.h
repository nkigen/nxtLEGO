#ifndef __SERVER_COMM_H__
#define __SERVER_COMM_H__

#include "../common/include/bt_packet.h"

/*
 * send Data to LEGO 
 */
void server_client_bt(bt_packet_t *in_packet,bt_packet_t *out_packet, int *bt_sock);
#endif
