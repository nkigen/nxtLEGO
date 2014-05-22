#ifndef __CLIENT_UTILITIES_H__
#define __CLIENT_UTILITIES_H__
#include <stdint.h>
#define TC		0.001
#define LP_ALPHA 	0.22
#define MAX_POWER	100
#define MIN_POWER	-100

/*Controller values*/
#define Kc	14
#define C	1		
#define B	-1.9648077
#define A	0.9650707
#define X	1
#define Y	-1.7633795
#define W	0.7633795

extern uint32_t K;
extern float e, e1, e2;
extern uint32_t u,u1,u2;
void init_controller();
float controllerUpdate();
float motorEncoder(uint32_t count);
uint32_t saturator(float val);
#endif
