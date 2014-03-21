
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
    return 0;
}


int bt_scan_devices(int s_sock,bt_device_t *devices[MAX_BT_DEVICES])
{
    long flags;
    int inq_res;
    char addr[BT_ADDR_LEN] = {0};
    char name[BT_NAME_LEN] = {0};
    inquiry_info *dev_in_range = NULL;

    memset(devices, 0, MAX_BT_DEVICES * sizeof(bt_device_t));
    dev_in_range = (inquiry_info *) malloc(sizeof(inquiry_info) * MAX_BT_DEVICES);

    inq_res = hci_inquiry(s_sock, MAX_INQUIRY_LEN, MAX_BT_DEVICES,NULL,&dev_in_range, IREQ_CACHE_FLUSH);

    if( inq_res < 0 )
    {
        perror("hci_inquiry error\n");
        return -1;
    }
    /*TODO: error when inq_res == 0*/

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
	strcpy(&devices[i]->name, name);
	memset(&devices[i]->device_addr,&dev_in_range[i].bdaddr,sizeof(bdaddr_t));
    }

    free(dev_in_range);
    close(s_sock);
    
    return inq_res;
}
