#ifndef __CONTROLLER_H__
#define __CONTROLLER_H__

#define MAX_POWER	100
#define MIN_POWER	-100
#define T		0.01

#define M_PI 		3.1415926f
/*Controller Values*/
#define K 		1 //to define
#define LP_ALPHA	0.4 
#define A		1
#define B		1
#define X		1
#define Y		1


typedef struct {
double e, e1, e2; /*Error values*/
double u, u1, u2; /*U*/
double dPrev; /*Previous value of derivative*/
double fPrev; /*Prev filter value*/
double cVel,dVel; /*current and desired velocity*/
}MOTOR_CONTROLLER;

extern MOTOR_CONTROLLER left_motor;
extern MOTOR_CONTROLLER right_motor;

void init_controller(MOTOR_CONTROLLER *c);
int saturator(double val);
double controllerUpdate(MOTOR_CONTROLLER *c, double error);
double derivative(MOTOR_CONTROLLER *c, double val);

#endif
