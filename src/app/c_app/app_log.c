#include <stdio.h>
#include <stdlib.h>

#include "include/app_log.h"

FILE *fp;
/*open the file */
int init_log(char *fname)
{
    if((fp=fopen(fname,"a+")) == NULL)
        return -1;
    return 0;
}
/*log the motor count values to file
 * format: <timestamp> <count> <motor power>*/
int log_motor_packet(char *fname, bt_packet_t *p)
{
    return fprintf(fp, "%f  %d\n", p->packets[0].data[TIMESTAMP_INDEX],(int) p->packets[0].data[VALUE_INDEX]);
}

int log_control_packet(bt_packet_t *p)
{
    return fprintf(fp, "%f %f\n",p->packets[0].data[VALUE_INDEX],p->packets[0].data[TIMESTAMP_INDEX]);
}
int end_log()
{
    return fclose(fp);
}

