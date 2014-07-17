#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include<inttypes.h>
#include<math.h>

#include "include/sensors.h"
#include "include/client_lego.h"
#include "include/client_req.h"
#include "../common/include/bt_packet.h"
#include "include/client_utilities.h"

#define unlikely(x)     __builtin_expect((x),0)
#define likely(x)       __builtin_expect((x),1)

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

uint32_t num_packets; /*Number of bt packets received/sent successfully*/

/*Control Variables*/
float desired_velocity_left;
float current_velocity_left;
float desired_velocity_right;
float current_velocity_right;

#define DEVICE_PWD "1234"

/******OSEK Declarations******/

DeclareCounter(SysTimerCnt);
DeclareTask(BtComm);
DeclareTask(SensorTask);
DeclareTask(LeftMotorControl);
DeclareTask(RightMotorControl);
DeclareTask(UnicycleControl);
DeclareTask(DisplayTask);
DeclareResource(RES_LCD);
DeclareAlarm(BT_COMM_ALARM);
DeclareAlarm(SENSOR_ALARM);
DeclareAlarm(LEFT_CONTROL_ALARM);
DeclareAlarm(RIGHT_CONTROL_ALARM);
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
    ecrobot_init_sonar_sensor(NXT_PORT_S4);
    timestamp = 0;
    bt_conn_status = 0;
    num_packets = 0;
    stream_size = 0;
    o_stream = 0;
    enable_streaming = 0;
    desired_velocity_left = 0;
    current_velocity_left = 0;
    desired_velocity_right = 0;
    current_velocity_right = 0;
    enable_controller = 1;
    reset_data_structs();
    reset_motor_power();
    init_controller();
    init_sensors();
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
	/*
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

    */
    outgoing_packet->packets[0].data[VALUE_INDEX] = sonar_ds;
    outgoing_packet->packets[0].data[TIMESTAMP_INDEX] = sonar_ts;
    bt_send((U8*)outgoing_packet, (U32)sizeof(bt_packet_t));
	
    TerminateTask();
}

TASK(UnicycleControl) {

#if 1
    if(enable_controller == 0)
        TerminateTask();
#endif

    desired_velocity_left = 1;
    desired_velocity_right = 1;
    TerminateTask();
}

TASK(SensorTask) {
    S32 val = ecrobot_get_sonar_sensor(NXT_PORT_S4);
    sonar_record(val, systick_get_ms());

    TerminateTask();
}

TASK(RightMotorControl) {
    float lc_update = 0.0;
    int val;
    //current_motor = NXT_PORT_A;
#if 1
    if(enable_controller == 0)
        TerminateTask();
#endif

    val = nxt_motor_get_count(NXT_PORT_B);
    /*If val is <= 0 -> Nothing to do just terminate in peace*/
    if(val == 0 && K_right > 0) 
        TerminateTask();
    current_velocity_right = motorEncoder(val, MOTOR_RIGHT);
    /*Update Error*/
    e_right = desired_velocity_right - current_velocity_right;

    /*Controller Implementation*/
    lc_update = controllerUpdate(MOTOR_RIGHT);
    u_right = saturator(lc_update);
    /*Update Plant*/
    int input = quantizer(u_right);
    if(input != 0 && input != quantizer(u1_right)) { /*Avoid setting same speed twice !!*/
        nxt_motor_set_speed(NXT_PORT_B, input, 1);
        e2_right = e1_right;
        e1_right = e_right;
        u2_right = u1_right;
        u1_right = u_right;
    }

    if(unlikely(K_right == UINT32_MAX))
	    K_right = 0;
    K_right++;
    TerminateTask();
}

TASK(LeftMotorControl) {
    float lc_update = 0.0;
    int val;
    //current_motor = NXT_PORT_A;
#if 1
    if(enable_controller == 0)
        TerminateTask();
#endif

    val = nxt_motor_get_count(NXT_PORT_C);
    if(val == 0 && K_left > 0)
        TerminateTask();
    current_velocity_left = motorEncoder(val, MOTOR_LEFT);
    /*Update Error*/
    e_left = desired_velocity_left - current_velocity_left;

    /*Controller Implementation*/
    lc_update = controllerUpdate(MOTOR_LEFT);
    u_left = saturator(lc_update);
    /*Update Plant*/
    int input = quantizer(u_left);
    if(input != 0 && input != quantizer(u1_left)) { /*Avoid setting same speed twice !!*/
        nxt_motor_set_speed(NXT_PORT_C, input, 1);
        e2_left = e1_left;
        e1_left = e_left;
        u2_left = u1_left;
        u1_left = u_left;
    }
    if(unlikely(k_left == UINT32_MAX))
	    k_left = 0;
    K_left++;
    TerminateTask();
}
TASK(DisplayTask)
{
    //  ecrobot_status_monitor("nxtLEGO client");
#if 1
    display_clear(1);
    display_goto_xy(1,0);
    display_string("nxtLEGO client");
    display_goto_xy(0,1);
    display_string("TS:");
    display_goto_xy(4,1);
    display_unsigned(timestamp,8);
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
    display_int(u_left,6);
#endif
    TerminateTask();
}
