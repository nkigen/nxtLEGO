

#include <stdio.h>
#include <string.h>

#include "include/app_comm.h"



int app_init_comm(int *server_sock)
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
