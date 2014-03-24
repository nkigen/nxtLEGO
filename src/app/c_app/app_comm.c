#include <stdio.h>
#include <string.h>

#include "include/app_comm.h"



int app_init_comm(int *c_sock)
{
    int rc;
    int len;
    int sd = -1; /*socket descriptor*/
    struct sockaddr_un addr;


    sd = socket(AF_UNIX, SOCK_STEAM, 0);
    if(sd < 0)
    {
        perror("c-app: socket failed");
        return -1;
    }

    *c_sock = sd;
    memset(addr, 0, sizeof(struct sockaddr));
    addr.sun_family = AF_UNIX;
    strcpy(addr.sun_path, SERVER_PATH);

    rc = connect(sd, (struct sockaddr *)&addr, sizeof(struct sockaddr));

    if(rc < 0)
    {
        perror("c-app: connect failed");
        return -1;
    }

    len = sizeof(bt_packet_t) * MAX_REQ;
    rc = setsockopt(sd, SOL_SOCKET, SO_RCVLOMAT, (char *)&len, sizeof(len));

    if(rc < 0)
    {
        perror("c-app: setsocketopt error");
        return -1;
    }
    return 0;
}


int app_terminate_comm(int c_sock)
{
    int rc;
    int len;
    bt_packet_t pkt[MAX_REQ];
    memset(pkt, 0, sizeof(bt_packet_t));

    pkt[0].operation = BT_END_CONNECTION;
    len = sizeof(bt_packet_t) * MAX_REQ;
    rc = send(c_sock, pkt, len, 0);
    if(rc < 0)
    {
        perror("c-app send error");
        return -1;
    }

    /*recv connection terminated confirmation*/
    rc = recv(c_sock, pkt, len, 0);

    if(rc < 0)
    {
        perror("c-app recv error");
        return -1;
    }

    close(c_sock);
    return 0;
}
