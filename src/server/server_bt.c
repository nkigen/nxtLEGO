#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <unistd.h>
#include <string.h>

#include "include/server_bt.h"

int bt_start_server(int *s_sock)
{
    int dev_id;

    dev_id  = hci_get_route(NULL);
    *s_sock = hci_open_dev(dev_id);

    if( dev_id < 0 || *s_sock < 0)
    {
        perror("error opening socket\n");
        return -1;
    }
    return dev_id;
}

int bt_connect_device(int *c_sock, bdaddr_t *c_addr)
{
    struct sockaddr_rc addr = { 0 };

    *c_sock = socket ( AF_BLUETOOTH, SOCK_STREAM, BTPROTO_RFCOMM);

    addr.rc_family = AF_BLUETOOTH;
    addr.rc_channel = (uint8_t)1;
    addr.rc_bdaddr = *c_addr;

    return  connect(*c_sock, (struct sockaddr*)&addr,sizeof(struct sockaddr));

}

int bt_scan_devices(int dev_id, int s_sock, bt_device_t *devices[MAX_BT_DEVICES])
{
    int flags = IREQ_CACHE_FLUSH;
    int inq_res;
    int i;
    char addr[BT_ADDR_LEN] = { 0 };
    char name[BT_NAME_LEN] = { 0 };
    inquiry_info *dev_in_range = NULL;

    //memset(devices, 0, MAX_BT_DEVICES * sizeof(bt_device_t));
    dev_in_range = (inquiry_info *) malloc(sizeof(inquiry_info) * MAX_BT_DEVICES);

    inq_res = hci_inquiry( dev_id, MAX_INQUIRY_LEN, MAX_BT_DEVICES, NULL, &dev_in_range, flags);

    if( inq_res < 0 )
    {
        perror("hci_inquiry error\n");
        return -1;
    }
    /*TODO: error when inq_res == 0*/

    printf("BT_Server: # Devices found: %d\n",inq_res);

    for(i=0; i<inq_res; ++i)
    {
        ba2str(&dev_in_range[i].bdaddr,addr);
        memset(name,0,BT_NAME_LEN);
        int r = hci_read_remote_name(s_sock, &dev_in_range[i].bdaddr, BT_NAME_LEN, name, 0);
        if( r < 0)
        {
            strcpy(name,"[unknown]\0");
        }
          
        devices[i] = (bt_device_t *) malloc(sizeof(bt_device_t));
        strcpy(devices[i]->name, name);
        memcpy(&devices[i]->device_addr,&dev_in_range[i].bdaddr,sizeof(bdaddr_t));

	printf("BT_Server: Device #: %d ; Device ID: %s ; Device Name: %s ;\n",i, addr,devices[i]->name);
    }

    free(dev_in_range);
    close(s_sock);

    return inq_res;
}

int bt_close_connection(int conn)
{
    return close(conn);
}
