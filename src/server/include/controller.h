#ifndef __CONTROLLER_H__
#define __CONTROLLER_H__
#include <stdint.h>
#include "../../common/include/bt_packet.h"
#include "server_bt.h"

/*********DEFINES*********/
#define SERVER_PATH		"/tmp/nxtLEGO/" /*Path to the UNIX socket*/
#define MAX_CONNECTIONS 	1 /*Maximum number of connections allowed to the server*/
/************GLOBALS*********/

void init_controller(int *server_sock);
int controller_accept_conn(int *server_sock, int *client_sock);
int controller_process_req(bt_packet_t *in, bt_packet_t *out,int *client_sock, int *bt_sock);
int controller_stop(int server_sock, int client_sock);
int controller_init_bt_conn( int *bt_server, bt_device_t *devices[MAX_BT_DEVICES]);
int controller_bt_stop(int server_sock, int client_sock);
#endif
