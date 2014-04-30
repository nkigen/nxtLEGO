#include <stdint.h>
#include <stdio.h>
#include "include/bt_packet.h"

void set_motor_power(bt_req_t *req, uint8_t  port, int power)
{
    req->operation 		= SET_MOTOR_POWER;
    req->port			= port;
    req->data[VALUE_INDEX]      = (float)power;
    req->data[TIMESTAMP_INDEX]  = 0;
}
/*When getting the motor power SPECIFY the number of samples to fetch from the LEGO*/
void get_motor_power(bt_req_t *req, uint8_t port)
{
    req->operation	   = GET_MOTOR_POWER;
    req->port		   = port;
}
void bt_packet_set_motor_power(bt_packet_t *p, uint8_t port, int power)
{
    set_motor_power(&p->packets[0], port, power);
}

void bt_packet_get_motor_power(bt_packet_t *p, uint8_t port)
{
    get_motor_power(&p->packets[0], port);

}

void bt_packet_end_stream(bt_packet_t *p) {
    p->packets[0].operation = BT_END_STREAMING;
    p->packets[0].port	= DEFAULT_PORT;
}
void bt_packet_start_stream(bt_packet_t *p, uint16_t nsamples) {
    p->packets[0].operation = BT_START_STREAMING;
    p->packets[0].data[VALUE_INDEX] = (float)nsamples;
    p->packets[0].port	= DEFAULT_PORT;
}

void bt_packet_end_connection(bt_packet_t *p) {
    p->packets[0].operation = BT_CLOSE_CONNECTION;
    p->packets[0].port		= DEFAULT_PORT;
}
void bt_print_packet(bt_packet_t *p)
{
    bt_req_t req = p->packets[0];
    printf("port: %d \nOperation:",req.port);
    switch(req.operation)
    {
    case GET_MOTOR_POWER:
        printf("GET MOTOR POWER\n");
        break;
    case SET_MOTOR_POWER:
        printf("SET MOTOR POWER value: %d\n",(int)req.data[VALUE_INDEX]);
        break;
    }
}

