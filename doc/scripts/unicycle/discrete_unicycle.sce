//Discrete
global uniKcD;
uniKcD = 0.9;
uniC = (s+0.4)/(s+1.2);
uniGD = ss2tf(dscr(uniC,uni_Tc));
