//Discrete
global uniKcD;
uniKcD = 4.1;
uniC = (s+0.1)/(s+2.1);
uniGD = ss2tf(dscr(uniC,uni_Tc));
