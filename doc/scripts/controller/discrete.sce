//Discritization
global C; 
global D_C;
global Tc;
LpALpha = 0.22;
Kc = 14;

//Controller
D_C = ss2tf(dscr(C,Tc));

