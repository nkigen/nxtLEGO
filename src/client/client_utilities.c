#include<stdio.h>
#include<math.h>
#include "client_utilities.h"

float e, e1, e2;
float PID, LC;
float K;
float I_k, D_k;
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
    PID = 0;
    LC = 0;
    e =  e1 = e2 = 0;
    I_k = D_k = 0;

}

inline float saturator(float val) {
    if(val >MAX_POWER)
        val = MAX_POWER;
    if(val < MIN_POWER)
        val = MIN_POWER;
    return val;
}

float PIDControllerUpdate() {
    float pid_u = (TC*PID + KP*e - KP*e1 + KI*TC*TC*e + KD*e - 2*KD*e1+ KD*e2)/TC;
    PID = pid_u;
    return pid_u;
}

float LCControllerUpdate() {
    float lc_u = LC_POLE*LC + e + LC_ZERO*e1;
    LC = lc_u;
    return lc_u;
}

inline float calcIntegral(float output) {
    float I =  I_k - TC*output;
    I_k = I;
    return I;
}

inline float quantizer(float val) {
    return floorf(val);
}

inline float LPFilter(float val) {
    float fd = LP_k + LP_k * LP_ALPHA  + LP_ALPHA*val;
    LP_k = fd;
    return fd;
}

inline float derivative(float val) {
    return (val - LP_k)/TC;
}

float motorEncoder(uint32_t count) {
    if(K == 0)
        return K;
    float integral = calcIntegral(count);
    float rad = DegToRad(integral);
    float quantized = quantizer(rad);
    float filt = LPFilter(quantized);
    float deg = RadToDeg(filt);
    return derivative(deg);

}

