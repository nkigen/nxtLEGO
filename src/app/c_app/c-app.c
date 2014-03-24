
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

#include "include/c-app.h"

/*******GLOBAL VARIABLES***********/
app_options_t options;


int start_app(char *buf, int size, int *c_sock)
{
    int rc;
    rc = decode_options(buff, &options);
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
