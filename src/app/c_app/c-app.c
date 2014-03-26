#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>

#include "include/c-app.h"
#include "include/app_log.h"

#define LOG_FILE			"c_app_log.dat"
#define DELAY_5_SECONDS()		 delay(5000)
#define ZERO_MOTOR_POWER(pkt,port)   	 bt_packet_set_motor_power(pkt, port, 0)	
/*******GLOBAL VARIABLES***********/
app_options_t options;

/*TODO:*/

static int get_next_motor_power(motor_opts_t *motor)
{
    int tmp;
    static int isFirst = 1;
    static int next = 0;
    static int last = 0;
    if(isFirst)
    {
        isFirst = 0;
        next = motor->min_power;
        last = motor->max_power;
    }
    if(next == motor->min_power)
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
    init_log(LOG_FILE);/*open the log file*/
    rc = decode_options(buf, &options);
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

int handler_get_motor_count(int c_sock, bt_packet_t *req, bt_packet_t *res, int count)
{
    int i;
    int rc;
    int power;
    int len;
    bt_packet_t motor_count[1];

    len =  sizeof(bt_packet_t);
    power = req->packets[0].data[VALUE_INDEX];
    memset(motor_count, 0, len);
    /*Send motor count req to server*/
    rc = send(c_sock, req, len, 0);
    if(rc < 0)
    {
        perror("failed to send motor packet");
        return -1;
    }

    /*receive ACK from client and ignore for now*/
    rc = recv(c_sock, res, len, 0);
    /*TODO: check rc value but we dont care for now*/
    /*prep packet for reply(num counts)*/
    memset(req, 0, len);/*reset the req packet*/
    memset(res, 0, len);
    bt_packet_get_motor_power(motor_count, req->packets[0].port);/*TODO: modify 0*/

    /**/
    do {

        rc = send(c_sock, req, len, 0);
        if(rc < 0)
        {
            perror("failed to send motor fetch  packet");
            return -1;
        }

        rc = recv(c_sock, res, len, 0);
        if( rc < 0)
        {
            perror("error receiving res packet");
        }
        else
        {
            /*Process the response( Log the values received)*/
            log_motor_packet(power, res);
        }

    } while(i++ < count);
    return 0;
}

int motor_handler(int c_sock, motor_opts_t *motor)
{
    int i = 0;
    int power;
    int _stop = 0;
    int last = motor->max_power;
    bt_packet_t request[1];
    bt_packet_t response[1];
    if(! motor->num_samples)/*Nothing to do !!*/
        return 0;

    do {
        power = get_next_motor_power(motor);
        if(power == last)
            _stop = 1;
        bt_packet_set_motor_power(request,motor->port, power); /*Prep packet for setting motor power*/
        handler_get_motor_count(c_sock, request, response, motor->num_samples);/*get and log the motor revs*/

    } while( !_stop);
}
/*TODO: Get a better name for this function!!!*/
int comm_handler(int c_sock)
{
    /*Now we only deal with motor_opts_t */
    motor_opts_t motor;
    memcpy(&motor, &options.motor, sizeof(motor_opts_t));

    motor_handler(c_sock, &motor);
}
