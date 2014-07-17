#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>

#include "include/c-app.h"
#include "include/app_log.h"

#define LOG_FILE			"log_power_\0"
#define CONTROL_LOG_FILE		"control_\0"

/*******GLOBAL VARIABLES***********/
app_options_t options;

/*TODO:*/

void get_control_log_file(int velocity, char *fname)
{
    strcpy(fname,CONTROL_LOG_FILE);
    char buf[10];

    sprintf(buf, "%d", velocity);
    strcat(fname,buf);
    printf("Using logfile:",fname);
    /*TODO:complete this*/
}
void get_log_file(int power, char *fname)
{
    strcpy(fname,LOG_FILE);
    char buf[10];

    sprintf(buf, "%d", power);
    strcat(fname,buf);
    printf("Using logfile:",fname);
    /*TODO:complete this*/
}
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
    options.m_control = {0,0};
	    options.motor = {0,0,0,0};
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
int handler_set_motor_power(int c_sock, bt_packet_t *req, bt_packet_t *res,motor_opts_t *motor,  int power) {

    bt_packet_set_motor_power(req,motor->port, power); /*Prep packet for setting motor power*/
    /*Send request to server*/
    int len;
    len =  sizeof(bt_packet_t);
    int rc = send(c_sock, req, len, 0);
    if(rc < 0)
    {
        perror("c-app: failed to send motor fetch  packet");
        return -1;
    }

    recv(c_sock, res, len, 0);  /*ignore the response*/
    return 0;
}
int handler_get_motor_count(int c_sock, bt_packet_t *req, bt_packet_t *res, motor_opts_t *motor, int count)
{
    int i;
    int rc;
    int len;
    len =  sizeof(bt_packet_t);
    i = 0;
    /**/
    printf("values to be fetched %d\n",count);
    bt_packet_get_motor_power(req,motor->port, count);
    rc = send(c_sock, req, len, 0);
    do {
        rc = recv(c_sock, res, len, 0);
        /*on recv erro just ignore the packet and bet your luck on the others*/
        if( rc < 0)
        {
            perror("c-app: error receiving res packet");
        }
        else
        {
            log_motor_packet(LOG_FILE, res);
        }
    } while(++i < count);

    /*TODO: reset motor power to zero the sleep for sometime before sending a request with a different power*/
    return 0;
}

int handler_start_motor_stream(int c_sock, bt_packet_t *req, bt_packet_t *res, uint16_t nsamples) {
    bt_packet_start_stream(req, nsamples);
    int len;
    len =  sizeof(bt_packet_t);
    int  rc = send(c_sock, req, len, 0);
    if(rc < 0)
    {
        perror("c-app: failed to send motor fetch  packet");
        return -1;
    }
    rc = recv(c_sock, res, len, 0);  /*ignore the response*/
    if(rc < 0)
        perror("c-app: stream reply not recvd");
    else
        printf("c-app: stream reply recvd\n");
    return 0;
}

    else
        printf("c-app: stream reply recvd\n");
    return 0;
}

int handler_end_connection(int c_sock, bt_packet_t *req, bt_packet_t *res) {
    bt_packet_end_connection(req);
    int len;
    len =  sizeof(bt_packet_t);
    int rc = send(c_sock, req, len, 0);
    if(rc < 0)
    {
        perror("c-app: failed to send motor fetch  packet");
        return -1;
    }
    recv(c_sock, res, len, 0);  /*ignore the response*/
    return 0;
}

int handler_end_motor_stream(int c_sock, bt_packet_t *req, bt_packet_t *res) {
    bt_packet_end_stream(req);
    int len;
    len =  sizeof(bt_packet_t);
    int rc = send(c_sock, req, len, 0);
    if(rc < 0)
    {
        perror("c-app: failed to send motor fetch  packet");
        return -1;
    }
    recv(c_sock, res, len, 0);  /*ignore the response*/
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
    if( motor->num_samples == 0)/*Nothing to do !!*/
    {
        printf("c-app: Nothing to do: samples ==0!!\n");
        return 0;
    }
    char fname[12];
    do {
        power = get_next_motor_power(motor);
        get_log_file(power,fname);
        init_log(fname);
        if(power == last)
            _stop = 1;

        handler_set_motor_power(c_sock, request, response, motor, power);
        handler_start_motor_stream(c_sock, request, response, motor->num_samples);

        handler_get_motor_count(c_sock, request, response, motor, motor->num_samples);/*get and log the motor revs*/
        handler_end_motor_stream(c_sock, request, response);
        /*Reset Motor to zero and pause for 3 seconds before changing the power value*/
        handler_set_motor_power(c_sock, request, response, motor, 0);
        end_log();
        sleep(3);
    } while(!_stop);
    /*Now send the BT_END_CONNECTION to terminate the server*/
    printf("c-app: Terminating connection to server...\n");
    handler_end_connection(c_sock, request, response);
    return 1;
}

inline int isControlMode() {
    return options.m_control.desired_velocity > 0;
}

int fetchControlData(int c_sock, bt_packet_t *req, bt_packet_t *res, int nsamples) {
    int i = 0;
    int rc;
    int len;
    len =  sizeof(bt_packet_t);
    char fname[12];
    bt_packet_start_control_stream(req);
    get_control_log_file(options.m_control.desired_velocity,fname);
    init_log(fname);
    rc = send(c_sock, req, len, 0);
    if(rc < 0)
    {
        perror("c-app: failed to start control stream  packet");
        //    return ;
    }
    rc = recv(c_sock, res, len, 0);
    while(i < nsamples) {
        rc = recv(c_sock, res, len, 0);
        if(rc < 0)
            perror("Fetch control data error");
        else
            log_control_packet(res);
        ++i;
    }
    end_log();
}

void startControlMode(int c_sock) {

    bt_packet_t request[1];
    bt_packet_t response[1];
    uint16_t sz = 1000;

    bt_packet_prep_control(request,options.m_control.port, options.m_control.desired_velocity, sz);
    int len;
    len =  sizeof(bt_packet_t);
    int rc = send(c_sock, request, len, 0);
    if(rc < 0)
    {
        perror("c-app: failed to send motor fetch  packet");

    }
    recv(c_sock, response, len, 0);  /*ignore the response*/

    /*Get Velocity Data*/
    fetchControlData(c_sock, request, response, sz);
    handler_end_connection(c_sock, request, response);

}
/*TODO: Get a better name for this function!!!*/
int comm_handler(int c_sock)
/*TODO: Implement this*/
int close_app()
{

}
