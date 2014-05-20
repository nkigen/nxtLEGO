//Discritization
global G; 
global C;
global D_G;
global D_C;
global D_PID;

global Tc;

global Kd;
global Kp;
global Ki;

Kp = 5;
Kd = 0;
LpALpha = 0.001;
Ki = 15;
s = poly(0, 's');
z = poly(0, 'z');

//Plant
D_G = ss2tf(dscr(G,Tc));

//Lead Compensator
D_C = ss2tf(dscr(C,Tc));

//PID Controller
C_PID = Kd*s + Ki/s + Kp;

D_PID = Kp + Ki/(1-z^-1) + Kd*(1-z^-1) //ss2tf(dscr(C_PID,Tc));

