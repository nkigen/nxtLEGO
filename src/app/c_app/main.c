#include <stdio.h>
#include <stdlib.h>

#include "include/c-app.h"

#define BUFFER_SIZE		128
/*
 * Assumption: buff is "BIG enough"
 */
static inline void agv2Buffer(char **argv, int argc, char *buff)
{
    int i = 0;
    char *p = buff;
    for( i=1; i < argc; ++i)
    {
        strcpy(p, argv[i]);
        p += strlen(argv[i]);
        *p = ' ';
        ++p;
    }
}

/*
 * close all open sockets and free any allocated memory
 * */
void on_terminate()
{
    printf("c-app: Garbage Collection");

    c_app_close();

}

int main(int argc, char **argv)
{
    atexit(on_terminate);
    int c_sock;/*client socket*/
    int rc;
    char buf[BUFFER_SIZE];
    motor_opts_t motor;
    agv2Buffer(argv, argc, buf);
    /*Decode argv and init connection to server*/
    rc = start_app(buf, BUFFER_SIZE, &c_sock);
    if(rc < 0)
    {
        perror("client: Failed to start c-app");
        return -1;
    }
    /*Prepare bt_packets to send with data from app_options_t
     * For now prepare only motor opts*/
    comm_handler(c_sock);
    return 0;
}
