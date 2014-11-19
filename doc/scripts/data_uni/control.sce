
tmax = 10;
step = 0.004;

fd = mopen("control_0","r");
if fd == -1 then
    error("Failed to open file for reading");
end
    isEOF = 0;
pos = 1;
data = [];
tdata=[];
d = 0;
t = 0;
while isEOF <> 1
    [n, d, t] = mfscanf(fd,"%f %f");
    if n>0 then
    data(pos) = d;
    tdata(pos) = t;
    pos = pos + 1;
else
    isEOF = 1;
    end
end
mclose(fd);
[pols, gain] = zpbutt(3,5);
H = gain/ (real(poly(pols,'s')));
H_C = ss2tf(dscr(H,0.004));
filt = filter(H_C.num,H_C.den,data);

for i = 1:length(filt)
    if filt(i)<0.4 then
        filt(i) =1 - filt(i);
    end
     if filt(i) > 0.5 then
        filt(i) = 0.5;
    end
end
filt(1) = 0;
plot(tdata,filt);
