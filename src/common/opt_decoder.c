#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "include/opt_decoder.h"

#define BUF_SIZE	16

static int decode_motor_power(char *str, motor_opts_t *motor)
{
    char *p = str;
    char tmp[5]; /*Maximum 3 digits+ 1 for the sign!!*/
    int val;
    int len;
    char *cp=p;
    if(( p = strchr(p, ':')) == NULL)
        return -1;
    len = p - &str[0];
    memcpy(tmp, str, len);
    sscanf(tmp, "%d", &val); /*TODO: check if > 0*/
    motor->min_power = val;
    ++p;
    cp = p;
    memset(tmp, 0, 5);
    if((p = strchr(p, ':')) == NULL)
        return -1;
    int sz = p - cp;
    memcpy(tmp, cp , sz);
    cp = p;
    sscanf(tmp,"%d",&val); /*TODO: check if > 0*/
    motor->max_power = val;
    p += 1;
    sscanf(p,"%d",&val);
    motor->step = val;
    return 0;
}
/*TODO: Seems not to decode -n >1000 well */
static int decode_motor_opts(char *str, app_options_t *opts)
{
    char *p = str;
    char buf[BUF_SIZE];
    float ctrl_vel;
   uint16_t tmp = 0;
    int n = 0;
    int isok = 0;
    while((p = strchr(p,'-')) != NULL && isok < NUM_MOTOR_OPTS)
    {
        switch(*(++p))
        {
        case OPT_PORT:
            ++isok;
            ++p;
            sscanf(p,"%d%n",&tmp,&n);
            opts->motor.port = tmp;
            p += n;
            break;
	case OPT_CONTROL_SPEED:
	    isok = NUM_MOTOR_OPTS;
	    ++p;
	    sscanf(p,"%f",&ctrl_vel);
	    opts->m_control.desired_velocity = ctrl_vel;
	    opts->m_control.port = opts->motor.port;
	    break;
        case OPT_MOTOR_POWER:
            ++isok;
            ++p;
            sscanf(p,"%s%n",buf,&n);
            p += n;
            decode_motor_power(buf,&opts->motor);
            break;
        case OPT_MOTOR_SAMPLE:
            ++isok;
            ++p;
            sscanf(p,"%d%n",&tmp,&n);
            opts->motor.num_samples = tmp;
            p += n;
            break;
        default:
            break;
        }

    }
    
    printf("min_power: %d , max_power: %d, step: %d samples %d\n", opts->motor.min_power,opts->motor.max_power,opts->motor.step, opts->motor.num_samples);
    printf("CONTROL PARAMS: %d %f\n",opts->m_control.port,opts->m_control.desired_velocity);
    if(isok == NUM_MOTOR_OPTS) /*All options are OK*/
        return 0;
    else         /*Sad case: Some important options are missing :-(*/
        return -1;
}

int decode_options(char *str, app_options_t *opts)
{
    int sz;
    sz = decode_motor_opts(str, opts);
    if(sz < 0)
    {
        perror("failed to decode options string");
        return -1;
    }
    else
        printf("Options decoded successfully\n");
    return 0;
}
