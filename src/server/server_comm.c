
#include <stdio.h>
#include <stdint.h>

#include "include/server_comm.h"

int server_client_bt(bt_packet_t *in_packet,bt_packet_t *out_packet, int *bt_sock)
{
    int res;
    int len = sizeof(bt_packet_t) * MAX_REQ;
    res = send( *bt_sock, in_packet, len, 0);
    if(res < 0)
    {
        perror("failed to send packet");
        return -1;
    }

    res = recv(*bt_sock, out_packet,len, 0);

    if(res < 0)
    {
        perror("failed to recv packet");
        return -1;
    }
    return 0;
}


