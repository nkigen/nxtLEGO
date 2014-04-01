#ifndef __BT_PACKET_H__
#define __BT_PACKET_H__
#include<stdint.h>

#define MAX_REQ			1 /*Maximum Number of requests send to/from the device. Also the number of 
				   concurrent requests/responses send/received*/
#define MAX_NUM_DATA		2 /*Maximum number of data transferred to/from LEGO( size of the data array in bt_req_t*/

/**
 * Index Definition for data array in bt_req_t
 * Note only for GET_MOTOR_COUNT operation are both
 * are both indices used
 */
#define VALUE_INDEX		0
#define TIMESTAMP_INDEX		1

/****PORT Definitions****/
#define PORT_1			1
#define PORT_2			2
#define PORT_3			3
#define PORT_4			4

#define MOTOR_A			5
#define MOTOR_B			6
#define MOTOR_C			7

/******PORT Operations*****/
#define BT_CLOSE_CONNECTION	102 /*Close Bluetooth Connection > Should not be within -100:100*/
#define SET_MOTOR_POWER		1 /*Set Motor power*/
#define GET_MOTOR_POWER		2 /*Get the number of "rotations" of the motor*/




/*
 * Defines Bluetooth requests for operations
 * to be performed by the Lego Brick
 */
 typedef struct {
    uint8_t operation;/*Operation to be performed*/
    uint8_t port; /**/
    float data[MAX_NUM_DATA];

} __attribute__((packed)) bt_req_t;


/*
 * Defines a Bluetooth packet send to the to/from the Lego brick
 */

typedef struct {
    uint32_t size; /*size of the packet (Always equal to MAX_REQ */
    bt_req_t packets[MAX_REQ];
} __attribute__((packed)) bt_packet_t;

#endif

