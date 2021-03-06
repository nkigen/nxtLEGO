global G;
global Kc;
global C;
global LpAlpha;
global Tc;

Tc = 0.001;

s = poly(0, 's'); 
//for right motor
//w_n =
//xi_n =
//for left motor
w_n = 7.4396976;
xi_n = 0.6474336;

function[xi_min] = getMinimumXI(desiredOvershoot)
    xi_min = sqrt(log(desiredOvershoot)^2/(%pi^2 + log(desiredOvershoot)^2));
endfunction

function[omega_min] = getMinimumOmega(alpha, desiredSettleTime, xi_min)
    N = 1/sqrt(1-xi_min^2);
    omega_min = (log(alpha/100) - log(N))/(-1*xi_min*desiredSettleTime);
endfunction

// Plant
G = 1/(s^2/w_n^2 + 2*(xi_n/w_n)*s +1);
//G = 1/(s*(s+4)*(s+6));

Kc = 100;
z0 = -3;
z1 = -20; 
p0 = -80; 
C =((s-z0)*(s-z1))/((s)*(s-p0));

xi_t = getMinimumXI(0.2);
w_t = getMinimumOmega(5,0.3,xi_n);

//clf();
//evans(C*G);
//sgrid(xi_t,w_t);
//
//kc_lim = kpure(syslin('c',C*G));
