#ifndef __CLIENT_UTILITIES_H__
#define __CLIENT_UTILITIES_H__
#include <stdint.h>
#define TC	0.001
#define LP_ALPHA 0.001
#define MAX_POWER	100
#define MIN_POWER	-100

/*PID Values*/
#define KP 	5
#define KI	30
#define KD	0.1

/*LEAD Compensator values*/
#define LC_ZERO		0.9952419
#define LC_POLE		0.9048374

extern uint32_t K;
extern float e, e1, e2;
extern float PID, LC;
void init_controller();
float PIDControllerUpdate();
float LCControllerUpdate();
float motorEncoder(uint32_t count);
float saturator(float val);
#endif
