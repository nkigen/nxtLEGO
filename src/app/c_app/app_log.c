
#include <stdio.h>
#include <stdlib.h>

#include "include/app_log.h"


FILE *fp;
/*open the file */
int init_log(char *fname)
{
    if((fp=fopen(fname,"wa")) == NULL)
        return -1;
    return 0;
}
/*log the motor count values to file
 * format: <timestamp> <count> <motor power>*/
int log_motor_packet(int power, bt_packet_t *p)
{

    printf( "%f  %d %d\n", p->packets[0].data[TIMESTAMP_INDEX],(int) p->packets[0].data[VALUE_INDEX], power);
    return fprintf(fp, "%f  %d %d\n", p->packets[0].data[TIMESTAMP_INDEX],(int) p->packets[0].data[VALUE_INDEX], power);

}
int end_log()
{
    return fclose(fp);
}

