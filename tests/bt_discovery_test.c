#include <stdio.h>
#include <stdlib.h>

#include "../src/server/include/server_bt.h"

int main(int argc, char **argv)
{
    int s_sock;
    bt_device_t *devices[MAX_BT_DEVICES];

    /*start server*/

    int ret = bt_start_server(&s_sock);

    if (ret == -1)
    {
        exit(EXIT_FAILURE);
    }

    bt_scan_devices(ret,s_sock, devices);

    return 0;
}
