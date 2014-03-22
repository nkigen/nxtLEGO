#include <stdio.h>

#include "include/client_lego.h"
//#include "../common/include/"
/*************GLOBAL VARIABLES**********/

uint32_t timestamp;
#define DEVICE_PWD "1234"

/******OSEK Declarations******/

DeclareTask(BtComm);
DeclareTask(DisplayTask);
DeclareResource(RES_LCD);
DeclareAlarm(BT_COMM_ALARM);
DeclareAlarm(LCD_UPDATE_ALARM);
DeclareCounter(SysTimerCnt);




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
    ecrobot_init_bt_slave(DEVICE_PWD);
}


void ecrobot_device_terminate()
{
    reset_motor_power();
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
