//Discretization
global C; 
global D_C;
global Tc;
LpALpha = 0.62;
Kc = 40;

//Controller
D_C = ss2tf(dscr(C,Tc));

