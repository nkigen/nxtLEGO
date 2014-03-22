#include <stdio.h>
#include <stdlib.h>


#include "include/client_lego.h"
#include "../common/include/bt_packet.h"


/*************GLOBAL VARIABLES**********/
uint32_t timestamp;
bt_packet_t incoming[MAX_REQ];
bt_packet_t outgoing[MAX_REQ];

#define DEVICE_PWD "1234"

/******OSEK Declarations******/

DeclareTask(BtComm);
DeclareTask(DisplayTask);
DeclareResource(RES_LCD);
DeclareAlarm(BT_COMM_ALARM);
DeclareAlarm(LCD_UPDATE_ALARM);
DeclareCounter(SysTimerCnt);


static void reset_data_structs()
{
    memset(incoming, 0, sizeof(bt_packet_t));
    memset(outgoing, 0, sizeof(bt_packet_t));
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
    timestamp = 0;
    reset_data_structs();
    reset_motor_power();
    ecrobot_init_bt_slave(DEVICE_PWD);
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
    /*check value of timestamp to prevent an overflow
     * if maximum value is reached reset it to 0*/
    if(unlikely (timestamp == (uintmax_t) UINT32_MAX))
        timestamp = 0;
    else
        ++timestamp;

    ercd = SignalCounter(SysTimeCnt);
    if(ercd != E_OK)
        ShutdownOS(ecrd);
}



TASK(BtComm)
{

    TerminateTask();
}


TASK(DisplayTask)
{

    TerminateTask();
}
