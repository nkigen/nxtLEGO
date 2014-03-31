
#include <stdio.h>
#include <stdint.h>

#include "include/server_comm.h"

int server_client_bt(bt_packet_t *in_packet,bt_packet_t *out_packet, int *bt_sock)
{
    printf("Trying to send BT packet to sd %d\n", *bt_sock);
    bt_print_packet(in_packet);
    int res;
    int len = sizeof(bt_packet_t);
    res = send( *bt_sock, in_packet, len, 0);
    if(res < 0)
    {
        perror("failed to send packet");
        return -1;
    }
    else
        printf("server: request sent to BT%d\n", res);

    res = recv(*bt_sock, out_packet,len, 0);

    if(res < 0)
    {
        perror("failed to recv packet");
        return -1;
    }
    else
        printf("server: response recved from BT %d\n", res);
    bt_print_packet(out_packet);
    return 0;
}


