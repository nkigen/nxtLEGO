#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <unistd.h>

#include "include/controller.h"

int controller_init(int *server_sock)
{
    struct sockaddr_un addr;
    int rc;
    *server_sock = socket(AF_UNIX, SOCK_STREAM, 0);

    if( *server_sock < 0)
    {
        perror("Cannot create server socket\n");
        return -1;
    }
    else
        printf("server: socket created\n");

    memset(&addr, 0, sizeof(struct sockaddr_un));

    addr.sun_family = AF_UNIX;
    strcpy(addr.sun_path, SERVER_PATH);
    printf("server: PATH used: %s\n",SERVER_PATH);
    /*** bind server to socket ***/
    unlink(SERVER_PATH);
    rc = bind(*server_sock, (struct sockaddr *)&addr, SUN_LEN(&addr));
    if(rc < 0)
    {
        perror("failed to bind server\n");
        return -1;
    }
    else
        printf("Server: socket bound successfully\n");

    rc = listen(*server_sock, MAX_CONNECTIONS);

    if (rc < 0)
    {
        perror("server listen failed");
        return -1;
    }
    else
	    printf("server: Listening for AF_UNIX connections");
    return 0;
}

int controller_init_bt_conn( int *bt_server, bt_device_t *devices[MAX_BT_DEVICES])
{
    int ret;
    ret = bt_start_server(bt_server);
    if(ret < 0)
    {
        perror("server:Failed to start bt_server");
        return -1;
    }
    else
        printf("server: bt server started\n");

    bt_scan_devices(ret, *bt_server, devices);
    return 0;
}
int controller_accept_conn(int *server_sock, int *client_sock)
{
    int len;
    int rc;
    *client_sock = accept(*server_sock, NULL, NULL);

    if (*client_sock < 0)
    {
        perror("server accept failed\n");
        return -1;
    }
    else
        printf("server: client %d accepted successfully\n", *client_sock);

    len = sizeof(bt_packet_t);
    rc = setsockopt(*client_sock, SOL_SOCKET, SO_RCVLOWAT,(char *) &len, sizeof(len));
    if(rc < 0)
    {
        perror("server: setsocketopt error");
        return -1;
    }
    else
	    printf("server: socket options set success!!\n");
    return 0;
}


int controller_process_req(bt_packet_t *in, bt_packet_t *out,int *client_sock, int *bt_sock)
{
	printf("server: Attempting to process request from %d...\n",*client_sock);
    int rc, len;
    len = sizeof(bt_packet_t);

    rc = recv(*client_sock, in, len, 0);
    if(rc < 0)
    {
        perror("server: recv error");
        return -1;
    }
    if( rc < len)
    {
        perror("incomplete data received from client");
        return -1;
    }
    else
	    printf("server: Client data received. NICE!!\n");

    /*TODO: cater for when in->size > 1*/
    if( (int)in->packets[0].data[VALUE_INDEX] != BT_CLOSE_CONNECTION)
    {
        server_client_bt(in,out,bt_sock); /*TODO: process return value*/
    }
    else
    {
        printf("Connection to client terminated");
        return 1; /*1 indicates connection has been terminated*/
    }

    /*send data back to client*/
    rc = send(*client_sock, out, len, 0);
    if(rc < 0)
    {
        perror("failed to send data to client");
        return -1;
    }
    return 0;
}

int controller_stop(int server_sock, int client_sock)
{
    if(server_sock != -1)
        close(server_sock);
    if(client_sock != -1)
        close(client_sock);

    unlink(SERVER_PATH);
}

int controller_bt_stop(int server_sock, int client_sock)
{
    if( client_sock != -1)
        bt_close_connection(client_sock);
    if( server_sock != -1)
        bt_close_connection(server_sock);
    return 0;
}
