#ifndef __DEVICE_OPERATIONS_H__
#define __DEVICE_OPERATIONS_H__

void bt_print_packet(bt_packet_t *p);

void set_motor_power(bt_req_t *req, uint8_t port, int power );
void get_motor_count(bt_req_t *req, uint8_t port);

void bt_packet_set_motor_power(bt_packet_t *p, uint8_t port, int power);
void bt_packet_get_motor_power(bt_packet_t *p, uint8_t port,int count);
void bt_packet_start_stream(bt_packet_t *p, uint16_t nsamples);
void bt_packet_end_stream(bt_packet_t *p);
void bt_packet_end_connection(bt_packet_t *p);
void bt_packet_prep_control(bt_packet_t *p, uint8_t port, float vel);
#endif

