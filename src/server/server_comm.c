
#include <stdio.h>
#include <stdint.h>

#include "include/server_comm.h"

int server_client_bt(bt_packet_t *in_packet,bt_packet_t *out_packet, int *bt_sock)
{
    printf("Trying to send BT packet to sd %d\n", *bt_sock);
//    bt_print_packet(in_packet);
    int res;
    int len = sizeof(bt_packet_t);
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
    //bt_print_packet(out_packet);
    return 0;
}

int server_send_bt(bt_packet_t *in, int *bt_sock) {
    printf("Trying to send BT packet to sd %d\n", *bt_sock);
  //  bt_print_packet(in_packet);
    int res;
    int len = sizeof(bt_packet_t);
    res = send( *bt_sock, in, len, 0);
    if(res < 0)
    {
        perror("failed to send packet");
        return -1;
    }
    return 0;

}

int server_recv_bt(bt_packet_t *out, int *bt_sock) {
int res;

    int len = sizeof(bt_packet_t);
    res = recv(*bt_sock, out,len, 0);
    if(res < 0)
    {
        perror("failed to recv packet");
        return -1;
    }
    return 0;

}
