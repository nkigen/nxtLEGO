#include "include/controller.h"



/*Motor Definition*/
MOTOR_CONTROLLER left_motor;
MOTOR_CONTROLLER right_motor;

UNICYCLE_CONTROLLER unicycle_controller;


/*Convert degrees to radians*/
double toRadians(int rad)
{
    return ( ( rad*MY_PI) / 180);
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
    c->u1 = 0;
    c->u2 = 0;
    c->dPrev = 0;
    c->cVel = 0;
    c->dVel = 0;
    c->fPrev = 0;
}

void initUnicycle(UNICYCLE_CONTROLLER *uc) {
    uc->w = 0.0;
    uc->w1 = 0.0;
    uc->e = 0.0;
    uc->e1 = 0.0;
    uc->A = 0.9901485;
    uc->B = 0.9704455;

}

void calcDesiredVelocity(MOTOR_CONTROLLER *rm, MOTOR_CONTROLLER *lm, UNICYCLE_CONTROLLER *uc) {

    /*Left Motor*/
    lm->dVel = ((2 * V) + (uc->cPos * D)) /(2 * R);
    /*right Motor*/
    rm->dVel = ((2 * V) - (uc->cPos * D)) /(2 * R);
}
/*Output: desired power*/
double controllerUpdate(MOTOR_CONTROLLER *c, double error) {
    /*TODO:implement this*/
    return 0.0;
}


/**
 * The Unicycle Controller implementation
 */
double unicycleUpdate(UNICYCLE_CONTROLLER *uc, double error) {
    uc->w = (uc->w1 - uc->e1 + uc->A*uc->e)*1/uc->B;
    uc->w1 = uc->w;
    uc->e1 = uc->e;
    uc->e = error;
    return uc->w;
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

