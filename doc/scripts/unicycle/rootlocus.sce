s = %s;
global uniC;
global uniKc;
global uniG;
//plant
uniG = v/s^2;

//controller

uniKc = 4.1;
uniC = (s+0.1)/(s+2.1);

//clf();
//evans(uniG*uniC, uniKc);
//rlocus();


