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
    p += len;
    cp = p;
    memset(tmp, 0, 5);
    if((p = strchr(p, ':')) == NULL)
        return -1;
    len = p - &str[0];
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
            ++p;
            sscanf(p,"%d%n",&tmp,&n);
            motor->port = tmp;
            p += n;
            break;
        case OPT_MOTOR_POWER:
            ++isok;
            ++p;
            sscanf(p,"%s%n",buf,&n);
            p += n;
            decode_motor_power(buf,motor);
            break;
        case OPT_MOTOR_SAMPLE:
            ++isok;
            ++p;
            sscanf(p,"%d%n",&tmp,&n);
            motor->num_samples = tmp;
            p += n;
            break;
        default:
            break;
        }

    }
    
    printf("min_power: %d , max_power: %d, step: %d samples %d\n", motor->min_power,motor->max_power,motor->step, motor->num_samples);
    if(isok == NUM_MOTOR_OPTS) /*All options are OK*/
        return 0;
    else         /*Sad case: Some important options are missing :-(*/
        return -1;
}

int decode_options(char *str, app_options_t *opts)
{
    int sz;
    sz = decode_motor_opts(str, &opts->motor);
    if(sz < 0)
    {
        perror("failed to decode options string");
        return -1;
    }
    else
        printf("Options decoded successfully\n");
    return 0;
}
