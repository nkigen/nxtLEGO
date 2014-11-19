function out=CalcDistance(in)
    d1 = in(1);
    d2 = in(2);
    v = in(3);
    tc = in(4);
    
dx = v*tc;
theta = atan((d2-d1)/dx);
h = d2*cos(theta);
out = [h];
endfunction
