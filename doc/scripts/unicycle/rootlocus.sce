s = %s;
global uniC;
global uniKc;
global uniG;
//plant
uniG = v/s^2;

//controller

uniKc = 2;
uniC = (s+0.4)/(s+2);

//clf();
//evans(uniG*uniC, uniKc);
//rlocus();


