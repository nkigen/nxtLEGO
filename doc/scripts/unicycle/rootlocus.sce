s = %s;
global uni_C;
global uni_Kc;
global uni_G;

global v;
v = 0.05;
//plant
uni_G = v/s^2;

//controller

uni_Kc = 3;
uni_C = (s+1)/(s+4);

//evans(uni_G*uni_C);
//rlocus();


