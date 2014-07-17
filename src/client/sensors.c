#include "include/sensors.h"
#include "include/cbArray.h"

ElemType e_sdata[SDATA_SIZE];
ElemType e_stime[SDATA_SIZE];
CircularBuffer sonar_data;
CircularBuffer sonar_time;
uint32_t sonar_ts; //sonar time difference
int32_t sonar_ds; //sonar distance difference

void init_sensors() {
    sonar_ds = 0;
    sonar_ts = 0;
    /*init sonar*/
    cbInit(&sonar_data, e_sdata, SDATA_SIZE);
    cbInit(&sonar_time,e_stime, SDATA_SIZE);

}


void sonar_record(int32_t val, uint32_t ts) {
    ElemType e = {val};
    ElemType e1 = {0};
//write distance
    cbWrite(&sonar_data, &e);
    cbPeep(&sonar_data, &e1);

    int32_t tmp  = e.value - e1.value;
    if(tmp < 0)
        tmp *= -1;
    sonar_ds = tmp;
//write dt
    e.value = ts;
    cbWrite(&sonar_time, &e);
    cbPeep(&sonar_time, &e1);

    sonar_ts = e.value - e1.value;
    if(sonar_ts < 0)
        sonar_ts *= -1;

}
