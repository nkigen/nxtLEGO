global G;
global Kc;
global C;
global Kp;
global Ki;
global Kd;
global Tc;
global CPID;
global LpAlpha;
Tc = 0.001;
s = poly(0, 's');
w_n = 28.400017;
xi_n = 0.2825585;
desiredOvershoot = 0.2;
desiredSettleTime = 0.3;

//Calculate Desired Parameters
xi_t = sqrt(log(desiredOvershoot)^2/(%pi^2 + log(desiredOvershoot)^2));
N = 1/sqrt(1-xi_t^2);
alpha = 5;
w_t = (log(alpha/100) - log(N))/(-1*xi_t*desiredSettleTime);

// Plant
G = 1/(s^2/w_n^2 + 2*(xi_n/w_n)*s +1);

//Lead Compensator

z0 = -5;
p0 = -100;
C = (s - z0)/((s - p0));

//PID Controller;
//Ki = 1;
//Kd = 1;
//Kp = 1;
//
//PI = (s+1)/s;
//PD = s+10;
//PP = Kp;
//
CPID = C;
Kc = 1/real(horner(CPID*G,[ 1 %i]* locate(1)));
//clf();
//evans(CPID*G);
//sgrid(xi_t,w_t);
//
//kc_lim = kpure(syslin('c',C*G));
