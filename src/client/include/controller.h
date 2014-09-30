#ifndef __CONTROLLER_H__
#define __CONTROLLER_H__

#define MAX_POWER	100
#define MIN_POWER	-100
#define T		(0.01)

#define MY_PI 		(3.1415926f)
/*Controller Values*/
#define K 		1 //to define
#define LP_ALPHA	0.4
#define A		1
#define B		1
#define X		1
#define Y		1

/*Unicycle controller Constant values*/
#define DESIRED_POSITION	(0.5) //Desired distance from the wall
#define V	3 //Foward Velocity
#define R	(0.028)
#define D	(0.11)

typedef struct {
    double e, e1, e2; /*Error values*/
    double u, u1, u2; /*U*/
    double dPrev; /*Previous value of derivative*/
    double fPrev; /*Prev filter value*/
    double cVel,dVel; /*current and desired velocity*/
} MOTOR_CONTROLLER;

typedef struct {
    double w; 
    double A;
    double B;
    double e1;
    double e;
    double w1; 
} UNICYCLE_CONTROLLER;

extern MOTOR_CONTROLLER left_motor;
extern MOTOR_CONTROLLER right_motor;
extern UNICYCLE_CONTROLLER unicycle_controller;

void init_controller(MOTOR_CONTROLLER *c);
void initUnicycle(UNICYCLE_CONTROLLER *uc);
double toRadians(int rad);
int saturator(double val);
double controllerUpdate(MOTOR_CONTROLLER *c, double error);
double unicycleUpdate(UNICYCLE_CONTROLLER *uc, double error);
double derivative(MOTOR_CONTROLLER *c, double val);
void calcDesiredVelocity(MOTOR_CONTROLLER *rm, MOTOR_CONTROLLER *lm, UNICYCLE_CONTROLLER *uc);
#endif
