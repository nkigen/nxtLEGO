#ifdef __BT_PACKET_H__
#define __BT_PACKET_H_

#define MAX_NUM_DATA	2 /*Maximum number of data transferred to/from LEGO*/


struct typedef {
    uint8_t operation;/*Operation to be performed*/
    uint8_t port; /**/
    float data[MAX_NUM_DATA];

} __attribute ((packed)) bt_packet_t;
#endif
