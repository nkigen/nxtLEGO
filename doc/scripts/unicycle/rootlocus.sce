s = %s;
global uni_C;
global uni_Kc;
global uni_G;


//plant
uni_G = v/s^2;

//controller

uni_Kc = 174;
uni_C = (s+4)/(s+40);

//evans(uni_G*uni_C);
//rlocus();


