
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

#include "include/c-app.h"

/*******GLOBAL VARIABLES***********/
app_options_t options;

static int get_next_motor_power(motor_opts_t *motor)
{
    int tmp;
    static int next = motor->min_power;
    static int last = motor->max_power;
    if(init == motor->min_power)
    {
        next += motor->step;
        return motor->min_power;
    }
    if((next + motor->step) <= last)
    {
        tmp = next;
        next += motor->step;
        return tmp;
    }
    return last;
}

int start_app(char *buf, int size, int *c_sock)
{
    int rc;
    rc = decode_options(buff, &options);
    if( rc < 0)
    {
        perror("Failed to decode the options");
        return -1;
    }
    rc =  app_init_comm(c_sock);
    if( rc < 0)
    {
        perror("Comms failed");
        return -1;
    }
    return 0;
}

int get motor_count(int c_sock, bt_packet_t *req, bt_packet_t *res, int count)
{
    int i;
    int rc;
    bt_packet_t motor_count[MAX_REQ];
    memset(motor_count, 0, sizeof(bt_packet_t));

    rc = send(c_sock, req, sizeof(bt_packet_t), 0);/*TODO: confirm this*/
    if(rc < 0)
    {
        perror("failed to send motor packet");
        return -1;
    }

    bt_packet_get_motor_power(motor_count, req->packets[0].port);/*TODO: modify 0*/

    do {



    } while(i++ < count);
    return 0;
}

int motor_handler(int c_sock, motor_opts_t *motor)
{
    int i = 0;
    int power;
    int _stop = 0;

    bt_packet_t request[MAX_REQ];
    bt_packet_t response[MAX_REQ];
    if(! motor->num_samples)/*Nothing to do !!*/
        return 0;

    do {
        power = get_next_motor_power(motor);
        if(power == last)
            _stop = 1;
        bt_packet_set_motor_power(request,motor->port, power); /*Prep packet for setting motor power*/
        get_motor_count(c_sock, request, response, motor->num_samples);/*get and log the motor revs*/

    } while( !_stop);
}
/*TODO: Get a better name for this function!!!*/
int comm_handler(bt_packet_t *req, bt_packet_t *req)
{
    /*Now we only deal with motor_opts_t */
    motor_opts_t motor;
    memcpy(&motor, &options.motor, sizeof(motor_opts_t));

    motor_handler(&motor);
}
