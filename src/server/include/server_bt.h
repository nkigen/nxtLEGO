#ifndef __SERVER_BT_H__
#define __SERVER_BT_H__

#include <sys/socket.h>
#include <bluetooth/bluetooth.h>
#include <bluetooth/rfcomm.h>
#include <bluetooth/hci.h>
#include <bluetooth/hci_lib.h>


#define MAX_BT_DEVICES		255 /*Max number of BT devices*/
#define BT_NAME_LEN 		248 /*MAX length of user readable bluetooth device name*/

/*
 * Structure representing a Bluetooth DEVICE
 */
typedef struct {
char name[BT_NAME_LEN]; /*user readable name of the bluetooth device*/

baddr_t device_addr; /*UNIQUE device address*/

}bt_device_t;


#endif
