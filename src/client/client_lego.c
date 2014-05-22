#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include<inttypes.h>
#include<math.h>

#include "include/client_lego.h"
#include "include/client_req.h"
#include "../common/include/bt_packet.h"
#include "include/client_utilities.h"

#define unlikely(x)     __builtin_expect((x),0)
/*************GLOBAL VARIABLES**********/
uint32_t timestamp;
uint32_t bt_conn_status;
uint16_t stream_size;
uint16_t o_stream; //debugging
uint8_t enable_streaming;
uint8_t current_motor;
uint8_t enable_controller;
bt_packet_t incoming_packet[1];
bt_packet_t outgoing_packet[1];

uint32_t num_packets; /*Number of bt packets received successfully*/

/*Controller Variables*/
float desired_velocity;
float current_velocity;

#define DEVICE_PWD "1234"

/******OSEK Declarations******/

DeclareCounter(SysTimerCnt);
DeclareTask(BtComm);
DeclareTask(ControllerTask);
DeclareTask(DisplayTask);
DeclareResource(RES_LCD);
DeclareAlarm(BT_COMM_ALARM);
DeclareAlarm(CONTROLLER_ALARM);
DeclareAlarm(LCD_UPDATE_ALARM);


static void reset_data_structs()
{
    memset(incoming_packet, 0, sizeof(bt_packet_t));
    memset(outgoing_packet, 0, sizeof(bt_packet_t));
}

static void reset_motor_power()
{
    nxt_motor_set_speed(NXT_PORT_A, 0, 1);
    nxt_motor_set_speed(NXT_PORT_B, 0, 1);
    nxt_motor_set_speed(NXT_PORT_C, 0, 1);
}


/****Initialize DEVICE*****/
void ecrobot_device_initialize()
{
    ecrobot_init_bt_slave(DEVICE_PWD);
    timestamp = 0;
    bt_conn_status = 0;
    num_packets = 0;
    stream_size = 0;
    o_stream = 0;
    enable_streaming = 0;
    desired_velocity = 6;
    current_velocity = 0;
    enable_controller = 0;
    reset_data_structs();
    reset_motor_power();
    init_controller();
}


void ecrobot_device_terminate()
{
    reset_motor_power();
    reset_data_structs();
    bt_reset();
    ecrobot_term_bt_connection();
}


void user_1ms_isr_type2(void)
{
    StatusType ercd;
#if 1
    /*check value of timestamp to prevent an overflow
    * if maximum value is reached reset it to 0*/
    /*TODO: Handle the time_stamp reset*/
    if(unlikely (timestamp == (uintmax_t) UINT32_MAX))
        timestamp = 0;
    else
        ++timestamp;
#endif
    ercd = SignalCounter(SysTimerCnt);
    if(ercd != E_OK)
        ShutdownOS(ercd);
}


TASK(BtComm)
{
    if(stream_size == 0 || enable_streaming == 1)
        bt_conn_status = bt_read((U8*)incoming_packet, 0, sizeof(bt_packet_t));

    if( bt_conn_status > 0)
    {
        ++num_packets;
        if(enable_streaming == 1)
            enable_streaming = 0;
        if(stream_size > 0)
            --stream_size;

        bt_decode_incoming(incoming_packet, outgoing_packet);
        bt_send((U8*)outgoing_packet, (U32)sizeof(bt_packet_t));
    }
    TerminateTask();
}

TASK(ControllerTask) {
    static float lc_update=0.0;
    float val;
 current_motor = NXT_PORT_A;
#if 0
    if(enable_controller == 0)
        TerminateTask();
#endif

    val = nxt_motor_get_count(current_motor);
    current_velocity = motorEncoder(val);
    /*Update Error*/
    e = desired_velocity - current_velocity;

    /*Controller Implementation*/
    lc_update = controllerUpdate();
    u = saturator(lc_update);
    /*Update Plant*/
    nxt_motor_set_speed(current_motor, u, 1);

    e2 = e1;
    e1 = e;
    u2 = u1;
    u1 = u;
    TerminateTask();
}
TASK(DisplayTask)
{
    // ecrobot_status_monitor("nxtLEGO client");
#if 1
    display_clear(1);
    display_goto_xy(1,0);
    display_string("nxtLEGO client");
    display_goto_xy(0,1);
    display_string("TS:");
#if 1
    display_goto_xy(4,1);
    display_unsigned(timestamp,8);
#endif
    display_goto_xy(1,2);
    display_string("Packets:");
    display_goto_xy(9,2);
    display_unsigned(num_packets,6);
    display_goto_xy(1,3);
    display_string("ssize:");
    display_goto_xy(9,3);
    display_unsigned(stream_size,6);
    display_goto_xy(1,5);
    display_string("power:");
    display_goto_xy(9,5);
    display_unsigned(u,6);
#endif
    TerminateTask();
}
