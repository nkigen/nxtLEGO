#ifndef __CLIENT_UTILITIES_H__
#define __CLIENT_UTILITIES_H__
#include <stdint.h>
#define TC		0.002
#define LP_ALPHA 	0.62
#define MAX_POWER	100
#define MIN_POWER	-100

/*Controller values*/
#define Kc	1//40
#define A	1.954
#define B	0.95424
#define X	1.84
#define Y	0.84

#define MOTOR_LEFT	0
#define MOTOR_RIGHT	1

extern uint32_t K_left, K_right;
extern float e_left, e1_left, e2_left;
extern float u_left,u1_left,u2_left;
extern float e_right, e1_right, e2_right;
extern float u_right,u1_right,u2_right;
void init_controller();
float controllerUpdate(int which);
float motorEncoder(int count, int which);
float saturator(float val);
int quantizer(float val);
#endif
