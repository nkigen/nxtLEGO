s = %s;
global uniC;
global uniKc;
global uniG;

global v;
v = 0.05;
//plant
uniG = v/s^2;

//controller

uniKc = 4;
uniC = (s+4)/(s+30);

//evans(uni_G*uni_C);
//rlocus();


