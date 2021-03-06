#include "include/controller.h"
#include<math.h>
#include "ecrobot_interface.h"


/*Motor Definition*/
MOTOR_CONTROLLER left_motor;
MOTOR_CONTROLLER right_motor;

UNICYCLE_CONTROLLER unicycle_controller;


/*Low Pass Filter*/
inline double filter(MOTOR_CONTROLLER *c, double input) {
    c->fPrev = (1 - LP_ALPHA)*c->fPrev + LP_ALPHA*input;
    return c->fPrev;
}
/*Convert degrees to radians*/
inline double toRadians(int rad)
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
    //uc->a = 0.9990104;
    //uc->b = 0.9792190;
    uc->a = 0.9984038;
    uc->b = 0.9952115;
    uc->cPos = 0.0;
    uc->pPos = 0.0;
}

void calcDesiredVelocity(MOTOR_CONTROLLER *rm, MOTOR_CONTROLLER *lm, UNICYCLE_CONTROLLER *uc) {
	rm->dVel = (uc->w*(D / (2*R) )) + V/R;
	lm->dVel =   V/R - (uc->w*(D / (2*R) ));
}

/*Output: desired power*/
double controllerUpdate(MOTOR_CONTROLLER *c, double error) {
    c->u = -c->u1*X - c->u2*Y + Kc*(error + c->e1*A + c->e2*B);
    //c->u = c->u1*X + c->u2*Y + Kc*(c->e + c->e1*A + c->e2*B);
    c->u2 = c->u1;
    c->u1 = c->u;
    c->e2 = c->e1;
    c->e1 = c->e;
    c->e = error;
    return c->u;
}


/**
 * The Unicycle Controller implementation
 */
double unicycleUpdate(UNICYCLE_CONTROLLER *uc, double error) {
    uc->w = (uc->w1 - uc->e1 + uc->a*error)*UNI_Kc/uc->b;
    uc->w1 = uc->w;
    uc->e1 = uc->e;
    uc->e = error;
    return uc->w;
}

double derivative(MOTOR_CONTROLLER *c, double val) {
    double der = (val - c->dPrev)/ T;
    c->dPrev = val;
    return filter(c,der);
}


double sensor_model(UNICYCLE_CONTROLLER *u) {
    double dt = 0.10*V;//systick_get_ms()*0.001*V;
    double dtheta = atan((u->cPos - u->pPos)/dt);
    u->pPos = u->cPos;
    return u->cPos*cos(dtheta);
}

