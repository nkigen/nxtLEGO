#include <stdio.h>

#include "kernel.h"
#include "kernel_id.h"
#include "ecrobot_interface.h"

#include "include/client_req.h"


static inline void bt_decode_port(uint8_t *in_port,uint8_t *out_port)
{
    switch(*in_port)
    {
    case MOTOR_A:
        *out_port = NXT_PORT_A;
        break;
    case MOTOR_B:
        *out_port = NXT_PORT_B;
        break;
    case MOTOR_C:
        *out_port = NXT_PORT_C;
        break;
    case PORT_1:
        *out_port = NXT_PORT_S1;
        break;
    case PORT_2:
        *out_port = NXT_PORT_S2;
        break;
    case PORT_3:
        *out_port = NXT_PORT_S3;
        break;
    case PORT_4:
        *out_port = NXT_PORT_S4;
        break;
    default:   /*TODO: Grave mistake if in_port ever lands here!!*/
        break;
    }
}
static inline void bt_req_process(bt_req_t *in, bt_req_t *out)
{
    uint8_t port;

    bt_decode_port(&in->port,&port);
    switch(in->operation)
    {
    case SET_MOTOR_POWER:
        nxt_motor_set_speed(port, in->data[VALUE_INDEX], 0);
	/*TODO: Send ACK to server after speed change*/
        break;
    case GET_MOTOR_COUNT:
        out->data[VALUE_INDEX] = nxt_motor_get_count(port);
        out->data[TIMESTAMP_INDEX] = timestamp;
        break;
    default:
        break;
    }
}

void bt_decode_incoming(bt_packet_t *incoming, bt_packet_t *outgoing)
{
    int i;

    for( i = 0; i < MAX_REQ; ++i)
    {
        bt_req_process(&incoming->packets[i], &outgoing->packets[i]);
    }
}
