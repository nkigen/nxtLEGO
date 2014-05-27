#include <stdio.h>
#include<string.h>
#include "kernel.h"
#include "kernel_id.h"
#include "ecrobot_interface.h"

#include "include/client_utilities.h"
#include "include/client_req.h"

#define GET_MOTOR_COUNT		GET_MOTOR_POWER
extern uint32_t timestamp;
extern float current_velocity;
extern uint16_t stream_size;
extern uint16_t o_stream;
extern uint8_t current_motor;
extern uint8_t enable_streaming;
extern float desired_velocity;
extern uint8_t enable_controller;
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

    out->port 	   = in->port;
    out->operation = in->operation;
    port = in->port;
    bt_decode_port(&in->port,&port);
    switch(in->operation)
    {
    case SET_MOTOR_POWER:
        nxt_motor_set_speed(port, in->data[VALUE_INDEX], 1);
	current_motor = port;
        /*TODO: Send ACK to server after speed change*/
        break;
    case GET_MOTOR_COUNT:
	out->data[VALUE_INDEX] = nxt_motor_get_count(port);
	out->data[TIMESTAMP_INDEX] = timestamp;
        //out->data[TIMESTAMP_INDEX] = timestamp;
       // out->data[TIMESTAMP_INDEX] = systick_get_ms();
        break;
    case BT_END_STREAMING:
        enable_streaming = 0;
        stream_size = 0;
        break;
    case BT_START_STREAMING:
      o_stream = stream_size =  in->data[VALUE_INDEX];
        enable_streaming = 1;
        break;
    case BT_CONTROL_MODE:
      o_stream = stream_size =  in->data[TIMESTAMP_INDEX];
	enable_controller = 1;
	enable_streaming = 1;
	current_motor = port;
	desired_velocity = in->data[VALUE_INDEX];
	break;
    case BT_CONTROL_STREAM:
        out->data[VALUE_INDEX] = current_velocity;
	out->data[TIMESTAMP_INDEX] = systick_get_ms();
//	out->data[TIMESTAMP_INDEX] = 
	break;
/*TODO: Add another case here for the velocity*/
    default:
        break;
    }
}

void bt_decode_incoming(bt_packet_t *incoming, bt_packet_t *outgoing)
{
    int i;
    /*Currently the only packet is for motor operations(One Motor)*/
    for( i = 0; i < MAX_REQ; ++i)
    {
        bt_req_process(&incoming->packets[i], &outgoing->packets[i]);
    }
}

void bt_handler(bt_packet_t *incoming, bt_packet_t *outgoing)
{
    int nruns = incoming->packets[0].data[TIMESTAMP_INDEX];
    if(!nruns) {
        bt_decode_incoming(incoming, outgoing);
        bt_send((U8*)outgoing, (U32)sizeof(bt_packet_t));
        return;
    }

    int i = 0;
    while(i < nruns) {
        bt_decode_incoming(incoming, outgoing);
        bt_send((U8*)outgoing, (U32)sizeof(bt_packet_t));
        ++i;
    }
}
