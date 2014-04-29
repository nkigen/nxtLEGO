#ifndef __SERVER_COMM_H__
#define __SERVER_COMM_H__

#include "../../common/include/bt_packet.h"
#include "../../common/include/device_operations.h"

/*
 * send Data to LEGO 
 */
int server_client_bt(bt_packet_t *in_packet,bt_packet_t *out_packet, int *bt_sock);
int server_recv_bt(bt_packet_t *out, int *bt_sock); 
int server_send_bt(bt_packet_t *in, int *bt_sock); 
#endif
