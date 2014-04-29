#ifndef __APP_LOG_H__
#define _APP_LOG_H__
#include "../../../common/include/bt_packet.h"

int init_log(char *fname);
int log_motor_packet(char *fname, bt_packet_t *p);
int end_log();

#endif
