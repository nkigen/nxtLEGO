#ifndef __CONTROLLER_H__
#define __CONTROLLER_H__

#define MAX_POWER	100
#define MIN_POWER	-100
#define T		(0.01)

#define MY_PI 		(3.1415926f)
/*Controller Values*/
#define Kc	40
#define UNI_Kc	(0.9)
#define LP_ALPHA 	0.62
#define A	(-1.9778667)
#define B	 (0.9779244)
#define X	(-1.9231163)
#define Y	 (0.9231163)

/*Unicycle controller Constant values*/
#define DESIRED_POSITION	(0.5) //Desired distance from the wall
#define V	(0.25) //Foward Velocity
#define R	(0.028) //wheel radius
#define D	(0.11) // distance between the wheels

typedef struct {
    double cPos; /*current sonar measurement*/
    double pPos; /*previous sonar measurement*/
    double e1, e;
    double w1, w;
    double a, b;
} UNICYCLE_CONTROLLER;

typedef struct {
    double e, e1, e2; /*Error values*/
    double u, u1, u2; /*U*/
    double dPrev; /*Previous value of derivative*/
    double fPrev; /*Prev filter value*/
    double cVel,dVel; /*current and desired velocity*/
} MOTOR_CONTROLLER;


extern MOTOR_CONTROLLER left_motor;
extern MOTOR_CONTROLLER right_motor;
extern UNICYCLE_CONTROLLER unicycle_controller;

void init_controller(MOTOR_CONTROLLER *c);
void initUnicycle(UNICYCLE_CONTROLLER *uc);
double toRadians(int rad);
int saturator(double val);
double unicycleSaturator(double val);
double controllerUpdate(MOTOR_CONTROLLER *c, double error);
double unicycleUpdate(UNICYCLE_CONTROLLER *uc, double error);
double derivative(MOTOR_CONTROLLER *c, double val);
void calcDesiredVelocity(MOTOR_CONTROLLER *rm, MOTOR_CONTROLLER *lm, UNICYCLE_CONTROLLER *uc);
double sensor_model(UNICYCLE_CONTROLLER *c);

#endif
