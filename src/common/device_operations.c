
#include <stdint.h>
#include "include/bt_packet.h"


void set_motor_power(bt_req_t *req, uint8_t  port, int power)
{
    req->operation 	= SET_MOTOR_POWER;
    req->port		= port;
    req->data[VALUE_INDEX] = (float)power;
}

void get_motor_power(bt_req_t *req, uint8_t port)
{
    req->operation	= GET_MOTOR_POWER;
    req->port		= port;
}
