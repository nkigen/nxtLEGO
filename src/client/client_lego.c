#include <stdio.h>

#include "include/client_lego.h"

/*************GLOBAL VARIABLES**********/

uint32_t timestamp = 0;


/******OSEK Declarations******/

DeclareTask(BtComm);
DeclareTask(DisplayTask);
DeclareResource(RES_LCD);
DeclareAlarm(BT_COMM_ALARM);
DeclareAlarm(LCD_UPDATE_ALARM);
DeclareCounter(SysTimerCnt);


void user_1ms_isr_type2(void)
{
    StatusType ercd;

    ++timestamp;

    ercd = SignalCounter(SysTimeCnt);
    if(ercd != E_OK)
        ShutdownOS(ecrd);
}



TASK(BtComm)
{

}


TASK(DisplayTask)
{

}
