#ifndef __SERVER_BT_H__
#define __SERVER_BT_H__

#include <sys/socket.h>
#include <bluetooth/bluetooth.h>
#include <bluetooth/rfcomm.h>
#include <bluetooth/hci.h>
#include <bluetooth/hci_lib.h>


#define MAX_INQUIRY_LEN		8
#define MAX_BT_DEVICES		255 /*Max number of BT devices*/
#define BT_NAME_LEN 		248 /*MAX length of user readable bluetooth device name*/
#define BT_ADDR_LEN	19

/*
 * Structure representing a Bluetooth DEVICE
 */
typedef struct {
    char name[BT_NAME_LEN]; /*user readable name of the bluetooth device*/

    baddr_t device_addr; /*UNIQUE device address*/

} bt_device_t;

/*
 * Search for available BLUETOOTH devices within range
 * upto a MAX of MAX_BT_DEVICES (defined above)
 */
int bt_scan_devices(int s_sock,bt_device_t *devices[MAX_BT_DEVICES]);


/*
 * Start Server Connection
 */

int bt_start_server(int *s_sock); 
/*
 * Start a Bluetooth Connection
 */
int bt_connect_device(int *c_sock,bdaddr_t *c_addr);
#endif
