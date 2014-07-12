#ifndef __SENSORS_H__
#define __SENSORS_H__

#include <stdint.h>
#define SDATA_SIZE	2

extern uint32_t sonar_ts;
extern int32_t sonar_ds;

void init_sensors();
void sonar_record(int32_t val, uint32_t ts);



#endif
