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
    printf("Input file %s\n", buf);
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
    else
	    printf("c-app: comm init success\n");
    return 0;
}

int handler_get_motor_count(int c_sock, bt_packet_t *req, bt_packet_t *res, int count)
{
    int i;
    int rc;
    int power;
    int len;

    len =  sizeof(bt_packet_t);
    power = req->packets[0].data[VALUE_INDEX];
    /*Send motor count req to server*/
    rc = send(c_sock, req, len, 0);
    if(rc < 0)
    {
        perror("failed to send motor packet");
        return -1;
    }
    else
	    printf("c-app: SET_MOTOR_POWER packet sent successfully\n");

    /*receive ACK from client and ignore for now*/
    rc = recv(c_sock, res, len, 0);

    if( rc > 0)
    {
         printf("c-app: ACK received. Good to go now!!\n");
    }
    else
	    printf("c-app: Nothing received from server\n");
    /*prep packet for reply(num counts)*/
    memset(res, 0, len);

    printf("c-app: preping GET_MOTOR_COUNT packet\n");
    bt_packet_get_motor_power(req, req->packets[0].port);/*TODO: modify 0*/
    /**/
    do {
        rc = send(c_sock, req, len, 0);
        if(rc < 0)
        {
            perror("c-app: failed to send motor fetch  packet");
            return -1;
        }
	else
		printf("c-app: Get motor count packet send successfully\n");

        rc = recv(c_sock, res, len, 0);

	/*on recv erro just ignore the packet and bet your luck on the others*/
        if( rc < 0)
        {
            perror("c-app: error receiving res packet");
        }
        else
        {
		printf("c-app: Motor Count Packet received successfully\n");
            /*Process the response( Log the values received)*/
            log_motor_packet(power, res);
        }

    } while(i++ < count);

   /*TODO: reset motor power to zero the sleep for sometime before sending a request with a different power*/
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

    do {
        power = get_next_motor_power(motor);
        if(power == last)
            _stop = 1;
	printf("c-app: setting motor power to %d\n", power);
        bt_packet_set_motor_power(request,motor->port, power); /*Prep packet for setting motor power*/
        handler_get_motor_count(c_sock, request, response, motor->num_samples);/*get and log the motor revs*/

    } while( !_stop);
}
/*TODO: Get a better name for this function!!!*/
int comm_handler(int c_sock)
{
    motor_handler(c_sock, &options.motor);
}
