#include<stdio.h>
#include<math.h>
#include "include/client_utilities.h"

float e, e1, e2;
float u,u1,u2;
uint32_t K;
float D_k; //prev value for derivative
float LP_k; //Previous value of filter

inline float DegToRad(float rad)
{
    return rad*M_PI/180;
}

inline float RadToDeg(float deg)
{
    return deg*M_PI/180;
}

void init_controller() {
    K = 0;
    e = 0;
    e1 = 0;
    e2 = 0;
    u = 0;
    u1 = 0;
    u2 = 0;
    LP_k =0;
    D_k = 0;
}
int quantizer(float val) {
    return (int)floorf(val);
}

float  saturator(float val) {
    if(val >MAX_POWER)
        val = MAX_POWER;
    else if(val < MIN_POWER)
        val = MIN_POWER;
    return val;
}



float controllerUpdate() {
    return  (-u1*X - u2*Y + Kc*(e + e1*A + e2*B*Kc));
}

inline float LPFilter(float val) {
    float fd = LP_k + LP_k * LP_ALPHA  - LP_ALPHA*val;
    LP_k = fd;
    return fd;
}

inline float derivative(float val) {
    float v =(val - D_k);///TC;
    D_k = val;
    return v;
}

float motorEncoder(uint32_t count) {

    float rad = DegToRad(count);
    //float filt = LPFilter(rad);
    return derivative(rad);
}


/*CB Array implementation*/
