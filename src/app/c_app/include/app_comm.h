#ifndef __APP_COMM_H__
#define __APP_COMM_H__
#include <stdint.h>
#include "../../../common/include/bt_packet.h"
#include "../../../server/include/server_comm.h"
#include "../../../server/include/controller.h"

int app_init_comm(int *server_sock);
int app_terminate_comm(int c_sock);
#endif
