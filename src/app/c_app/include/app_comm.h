#ifndef __APP_COMM_H__
#define __APP_COMM_H__

#include "../../common/include/bt_packet.h"
#include "../../server/include/server_comm.h"


int app_init_comm(int *server_sock);
int app_terminate_comm(int c_sock);
#endif
