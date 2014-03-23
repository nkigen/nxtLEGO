#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "include/opt_decoder.h"

#define BUF_SIZE	16

static inline int decode_motor_power(char *str, motor_opts_t *motor)
{
    char *p =str;
    char tmp[5]; /*Maximum 3 digits+ 1 sign digit!!*/
    int val;
    int len;
    p = strchr(p,':');
    if(p != NULL) {
        len = p - &str[0];
        memcpy(tmp,str,len);
        sscanf(tmp,"%d",&val); /*TODO: check if > 0*/
        motor->min_power = val;
        p += len + 1;
    }
    memset(tmp, 0, 5);
    p = strchr(p,':');
    if(p != NULL) {
        len = p - &str[0];
        memcpy(tmp,str,len);
        sscanf(tmp,"%d",&val); /*TODO: check if > 0*/
        motor->max_power = val;
        p += len + 1;
    }

    sscanf(p,"%d",&val);
    motor->step = val;
}
static int decode_motor_opts(char *str, motor_opts_t *motor)
{
    char *p = str;
    char buf[BUF_SIZE];
    int tmp = 0;
    int n = 0;
    int isok = 0;
    while((p = strchr(p,'-')) != NULL && isok < NUM_MOTOR_OPTS)
    {
        switch(*(++p))
        {
        case OPT_PORT:
            ++isok;
            sscanf(p,"%d%n",&tmp,&n);
            motor->port = tmp;
            p += n;
            break;
        case OPT_MOTOR_POWER:
            ++isok;
            sscanf(p,"%s%n",buf,&n);
            p += n;
            decode_motor_power(buf,motor);
            break;
        case OPT_MOTOR_SAMPLE:
            ++isok;
            sscanf(p,"%d%n",&tmp,&n);
            motor->num_samples = tmp;
            p += n;
            break;
        default:
            break;
        }

    }
    if(isok == NUM_MOTOR_OPTS)
        return 0;
    else
        return -1;


}

int decode_options(char *str, app_options_t *opts)
{
    int sz;
    motor_opts_t motor;
    memset(&motor, 0 , sizeof(motor_opts_t));

    sz = decode_motor_opts(str, &motor);

    if(sz < 0)
    {
        perror("failed to decode str");
        return -1;
    }

}
