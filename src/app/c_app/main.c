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
    for( i=0; i < argc; ++i)
    {
        strcpy(p,argv[i]);
        p + = strlen(argv[i]);
        *p = ' ';
        ++p;
    }
}


int main(int argc, char **argv)
{
    int c_sock;
    char buf[BUFFER_SIZE];
    argv2buff(argv, argc, buff);

}
