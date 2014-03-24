#ifndef __DEVICE_OPERATIONS_H__
#define __DEVICE_OPERATIONS_H__


void set_motor_power(bt_req_t *req, uint8_t port, int power );
void get_motor_count(bt_req_t *req, uint8_t port);

void bt_packet_set_motor_power(bt_packet_t *p, uint8_t port, int power);
void bt_packet_get_motor_power(bt_packet_t *p, uint8_t port);
#endif

