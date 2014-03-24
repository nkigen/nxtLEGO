#ifndef __C_APP_H__
#define __C_APP_H__

#include "../server/include/controller.h"
#include "../common/include/device_operations.h"
#include "../common/include/opt_decoder.h"
#include "app_comm.h"
extern app_options_t options;
int start_app(int argc, char **argv, int *c_sock);
int comm_handler(bt_packet_t *req, bt_packet_t *req);
int motor_handler(motor_opts_t *motor);
#endif
