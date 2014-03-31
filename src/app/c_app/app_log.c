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
int log_motor_packet(char *fname, int power, bt_packet_t *p)
{
    if(init_log(fname) == 0) {
        printf( "%f  %d %d\n", p->packets[0].data[TIMESTAMP_INDEX],(int) p->packets[0].data[VALUE_INDEX], power);
        fprintf(fp, "%f  %d %d\n", p->packets[0].data[TIMESTAMP_INDEX],(int) p->packets[0].data[VALUE_INDEX], power);
        end_log();
        return 0;
    }
    return -1;

}

int end_log()
{
    return fclose(fp);
}

