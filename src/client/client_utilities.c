#include<stdio.h>
#include<math.h>
#include "include/client_utilities.h"

float e, e1, e2;
uint32_t u,u1,u2;
uint32_t K;
float I_k;
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
    e =  e1 = e2 = 0;
    u = u1 = u2 = 0;
    I_k = LP_k = D_k = 0;

}
inline float quantizer(float val) {
    return floorf(val);
}

inline uint32_t saturator(float val) {
    if(val >MAX_POWER)
        val = MAX_POWER;
    else if(val < MIN_POWER)
        val = MIN_POWER;
    return quantizer(val);
}



float controllerUpdate() {
    float lc_u =  -u1*Y - u2*W + e*Kc + e1*B*Kc + e2*A*Kc;
    return lc_u;
}

inline float calcIntegral(float output) {
    float I =  I_k - TC*output;
    I_k = I;
    return I;
}

inline float LPFilter(float val) {
    float fd = LP_k + LP_k * LP_ALPHA  - LP_ALPHA*val;
    LP_k = fd;
    return fd;
}

inline float derivative(float val) {
	float v =(val - D_k)/TC;
	D_k = val;
    return v; 
}

float motorEncoder(uint32_t count) {
#if 0
    float integral = calcIntegral(count);
    float rad = DegToRad(integral);
    float quantized = quantizer(rad);
    float filt = LPFilter(quantized);
    float deg = RadToDeg(filt);
#endif

    float rad = DegToRad(count);
    float filt = LPFilter(rad);
    return derivative(filt);
}

