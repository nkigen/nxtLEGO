#ifdef __BT_PACKET_H__
#define __BT_PACKET_H_

#define MAX_REQ		1 /*Maximum Number of requests send to/from the device*/
#define MAX_NUM_DATA		2 /*Maximum number of data transferred to/from LEGO*/

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
#define SET_MOTOR_POWER		0
#define GET_MOTOR_COUNT		1




/*
 * Defines Bluetooth requests for operations
 * to be performed by the Lego Brick
 */
struct typedef {

    uint8_t operation;/*Operation to be performed*/
    uint8_t port; /**/
    float data[MAX_NUM_DATA];

} __attribute__((packed)) bt_req_t;


/*
 * Defines a Bluetooth packet send to the to/from the Lego brick
 */

typedef struct {

    uint32_t size; /*size of the */
    bt_req_t packets[MAX_REQ]

} __attribute__((packed)) bt_packet_t;

#endif

