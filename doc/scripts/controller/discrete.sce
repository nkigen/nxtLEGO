//Discritization
global C; 
global D_C;
global Tc;
Kp = 10;
Kd = 0;
LpALpha = 0.22;

//Controller
D_C = ss2tf(dscr(C,Tc));

