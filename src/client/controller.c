#include "include/controller.h"



/*Motor Definition*/
MOTOR_CONTROLLER left_motor;
MOTOR_CONTROLLER right_motor;

/*Convert degrees to radians*/
double toRadians(int rad)
{
    return ( ( rad*M_PI) / 180);
}

/*Saturate power values*/
int  saturator(double val) {
    if(val > MAX_POWER)
        val = MAX_POWER;
    else if(val < MIN_POWER)
        val = MIN_POWER;
    return (int)val;
}

void init_controller(MOTOR_CONTROLLER *c) {
    c->e = 0;
    c->e1 = 0;
    c->e2 = 0;
    c->u = 0;
    c-u1 = 0;
    c->u2 = 0;
    c->dPrev = 0;
    c->cVel = 0;
    c->dVel = 0;
    c->fPrev = 0;
}

/*Output: desired power*/
double controllerUpdate(MOTOR_CONTROLLER *c, double error) {
    /*TODO:implement this*/
}


double derivative(MOTOR_CONTROLLER *c, double val) {
    c->dPrev = (val - c->dPrev)/ T;
    return c->dPrev;
}

/*Low Pass Filter*/
double filter(MOTOR_CONTROLLER *c, double input) {
    c->fPrev = (1 - LP_ALPHA)*c->fPrev + LP_ALPHA*input;
    return c->fPrev;
}
