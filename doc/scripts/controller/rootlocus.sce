global G;
global Kc;
global C;
s = poly(0, 's');
w_n = 28.400017;
xi_n = 0.2825585;

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

//Lead Compensator
Kc = 9;
z0 = -5;
p0 = -100;
C = (s - z0)/((s - p0));

xi_t = getMinimumXI(0.2);
w_t = getMinimumOmega(5,0.3,xi_n);

clf();
evans(C*G);
sgrid(xi_t,w_t);

//kc_lim = kpure(syslin('c',C*G));
