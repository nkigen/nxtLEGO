// (z-1)/t

function[der] = ZDerivative(u,t)
    z = poly(0,'z');
    der = (z -1)/t;
endfunction
