s = %s;
global uni_C;
global uni_Kc;
global uni_G;
v = 4;

//plant
uni_G = v/s^2;

//controller

uni_Kc = 50;
uni_C = (s+10)/(s+30);

//evans(uni_G*uni_C);
//rlocus();


