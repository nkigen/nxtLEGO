#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include<inttypes.h>
#include<math.h>
#include "include/client_lego.h"
#include "include/client_req.h"
#include "../common/include/bt_packet.h"
#include "include/controller.h"

#define unlikely(x)     __builtin_expect((x),0)
#define likely(x)       __builtin_expect((x),1)

/*************GLOBAL VARIABLES**********/
uint32_t timestamp;
uint32_t bt_conn_status;
uint16_t stream_size;
uint16_t o_stream; //debugging
uint8_t enable_streaming;
bt_packet_t incoming_packet[1];
bt_packet_t outgoing_packet[1];

uint32_t num_packets; /*Number of bt packets received/sent successfully*/


#define DEVICE_PWD "1234"

/******OSEK Declarations******/

DeclareCounter(SysTimerCnt);
DeclareTask(BtComm);
DeclareTask(MotorControl);
DeclareTask(UnicycleController);
DeclareTask(DisplayTask);
DeclareResource(RES_LCD);
DeclareAlarm(BT_COMM_ALARM);
DeclareAlarm(CONTROL_ALARM);
DeclareAlarm(UNICYCLE_ALARM);
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


/*Position Estimation Algorithm*/

double getPositionFromWall(UNICYCLE_CONTROLLER *uc) {
    static double sonar;
    sonar = ecrobot_get_sonar_sensor(NXT_PORT_S4);

    return sensor_model(uc);
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
    reset_data_structs();
    reset_motor_power();
    init_controller(&left_motor);
    init_controller(&right_motor);
    initUnicycle(&unicycle_controller);
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
    /*USed when in stream mode
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
    TerminateTask();
}

TASK(MotorControl) {
    static double pos;
    static double power;
    static double pos_l;
    static double power_l;
    pos = nxt_motor_get_count(NXT_PORT_A);
    pos = toRadians(pos); /*Convert pos to radians*/
    pos_l = nxt_motor_get_count(NXT_PORT_B);
    pos_l = toRadians(pos_l); /*Convert pos to radians*/
    /*Get current speed (Filtered)*/
    derivative(&right_motor, pos);
    derivative(&left_motor, pos_l);

    /*Calculate Desired Velocity(From unicycle Output)*/
    calcDesiredVelocity(&right_motor, &left_motor, &unicycle_controller);

    /*Update the controller*/
    power = controllerUpdate(&right_motor,(right_motor.dVel - right_motor.cVel));
    power_l = controllerUpdate(&left_motor,(left_motor.dVel - left_motor.cVel));

    /*Update motor power*/
    nxt_motor_set_speed(NXT_PORT_A,saturator(power), 1);
    nxt_motor_set_speed(NXT_PORT_B,saturator(power_l), 1);
    TerminateTask();
}

TASK(UnicycleController) {
    static double sonar;
    sonar = getPositionFromWall(&unicycle_controller);
    unicycleUpdate(&unicycle_controller, (DESIRED_POSITION - unicycle_controller.cPos) );
}

TASK(DisplayTask)
{
      ecrobot_status_monitor("nxtLEGO client");
#if 0
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
    display_unsigned(K_left,6);
    display_goto_xy(1,5);
    display_string("power:");
    display_goto_xy(9,5);
    display_int(u_left,6);
#endif
    TerminateTask();
}
