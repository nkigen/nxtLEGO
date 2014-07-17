#include<stdio.h>
#include<math.h>
#include "include/client_utilities.h"
#include "include/cbArray.h"
float e_left, e1_left, e2_left;
float e_right, e1_right, e2_right;
float u_left,u1_left,u2_left;
float u_right,u1_right,u2_right;
uint32_t K_left, K_right;
ElemType elems_left[DER_BUFFER]; //prev value for derivative
ElemType elems_right[DER_BUFFER]; //prev value for derivative
CircularBuffer cb_right;
CircularBuffer cb_left;
float LP_k_left; //Previous value of filter
float LP_k_right; //Previous value of filter

inline float DegToRad(uint32_t rad)
{
    return ( ((float) rad*M_PI) / 180);
}

inline float RadToDeg(float deg)
{
    return deg*M_PI/180;
}

void init_controller() {
    K_left = 0;
    e_left = 0;
    e1_left = 0;
    e2_left = 0;
    u_left = 0;
    u1_left = 0;
    u2_left = 0;
    LP_k_left =0;

    K_right = 0;
    e_right = 0;
    e1_right = 0;
    e2_right = 0;
    u_right = 0;
    u1_right = 0;
    u2_right = 0;
    LP_k_right =0;
    cbInit(&cb_left,elems_left,DER_BUFFER);
    cbInit(&cb_right,elems_right,DER_BUFFER);
}
int quantizer(float val) {
    return (int)floorf(val);
}

float  saturator(float val) {
    if(val > MAX_POWER)
        val = MAX_POWER;
    else if(val < MIN_POWER)
        val = MIN_POWER;
    return val;
}



float controllerUpdate(int which) {
    if(which == MOTOR_LEFT)
        return  (-u1_left*X - u2_left*Y + Kc*(e_left + e1_left*A + e2_left*B));
    if(which == MOTOR_RIGHT)
        return  (-u1_right*X - u2_right*Y + Kc*(e_right + e1_right*A + e2_right*B));
    /*CRITICAL ERROR: sHOULD nEVeR HapPEN*/
    return 0;
}

inline float LPFilter(float val, int which) {
    float fd=0;
    if(which == MOTOR_LEFT) {
        fd  = LP_k_left + LP_ALPHA * (LP_k_left - val);
        LP_k_left = fd;
    }
    if(which == MOTOR_RIGHT) {
        fd  = LP_k_right + LP_ALPHA * (LP_k_right - val);
        LP_k_right = fd;
    }
    return fd;
}

inline float derivative(float val, int which) {
    ElemType e = {val};
    ElemType e1 = {0};

    if(which == MOTOR_LEFT) {
        cbWrite(&cb_left,&e);
        cbPeep(&cb_left,&e1);
    }
    else if(which == MOTOR_RIGHT) {
        cbWrite(&cb_right,&e);
        cbPeep(&cb_right,&e1);
    }
    return ((e.value - e1.value)/(TC*DER_BUFFER));
}

float motorEncoder(int count,int which) {
    float rad = DegToRad(count);
    float der = derivative(rad, which);
    float filt = LPFilter(der, which);
    return filt;
}
