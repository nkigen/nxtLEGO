#include <stdio.h>
#include <stdlib.h>

#include "../../server/include/controller.h"


/*
 * Function to close all open sockets and cleanup
 * Whenever the program terminates*/

int server_sock;
int client_sock;

int bt_server_sock; /*Bluetooth server socket*/
int bt_sock; /*bluetooth connection to LEGO*/
void on_terminate()
{
    /*TODO:add this*/
    controller_stop(server_sock, client_sock);
    controller_bt_stop(bt_server_sock, bt_sock);
}
int main(int argc, char **argv)
{
    /*register on_terminate*/
    atexit(on_terminate);

    int rc;
    int conn_status = 0; /***/
    int isconn = 0;

    bt_packet_t incoming[1];
    bt_packet_t outgoing[1];

    bt_device_t *devices[MAX_BT_DEVICES];

    memset(incoming, 0, sizeof(bt_packet_t));
    memset(outgoing, 0, sizeof(bt_packet_t));

    /*Initialize and scan for bluetooth devices in range*/
    rc = controller_init_bt_conn(&bt_server_sock, devices);
    if(rc < 0)
    {
        perror("server: failed to initialize bluetooth connection");
        return -1;
    }
    else
        printf("server: initialized bluetooth\n");

    /*TODO:URGENT !!!  pick one of the bluetooth devices and initiate a connection*/
    int pos,i = 0;
    while(i < MAX_BT_DEVICES)
    {
	    if(devices[i]->name[0] == 'D')
	    {
		    pos = i;
		    break;
	    }
	    else
		    i++;
    }
    rc =  bt_connect_device(&bt_sock, &devices[0]->device_addr);
    if(rc < 0)
    {
        perror("server: failed to Connect to bluetooth device");
        return -1;
    }

    /*Start server*/
    printf("server: starting server....\n");
    rc = controller_init(&server_sock);
    if(rc < 0)
    {
        perror("server: failed to initialize");
        return -1;
    }
    else
        printf("server: initilization complete....\n");

    /*wait for connections*/

    printf("server: waiting for connection...\n");
    rc = controller_accept_conn(&server_sock, &client_sock);
    if( rc < 0)
    {
        perror("server: error accepting connection");
        return -1;
    }
    else
    {
        printf("server: connection accepted...\n");
    }

    do {

        printf("server: waiting for requests...\n");
        rc = controller_process_req(incoming, outgoing, &client_sock, &bt_sock);
        if( rc < 0)
        {
            perror("server: error processing request");
        }
        else
            printf("server: request processed..\n");

        if(rc == 1)/*connection has been terminated*/
            conn_status = rc;

    } while(!conn_status);

    /*close connections*/
    return 0;
}
