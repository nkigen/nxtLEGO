s = %s;
global uni_C;
global uni_Kc;
global uni_G;


//plant
uni_G.num = v;
uni_G.den = s^2;

//controller

uni_Kc = 174;
uni_C.num = s+4;
uni_C.den = s+40;

//evans(uni_g*uni_C);
//rlocus();


