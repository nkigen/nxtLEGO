//used to verify the model params
global omega_n;
global xi_n; 
global q_n; 
global g_n; 
global fname;
global imgname;
global CURRENT_POWER; //Current power being analysed 
global StepValue;
StepValue = 0.001; 
//Get the verification signal
   
sAlpha = 0.05; //Settling time alpha (5%)
global StepValue;
StepValue = 0.001;  
    
//Open and Read file
fd = mopen(fname,'r');
if fd == -1 then
    error("Failed to open file for reading");
end

//read input file containing the data
//format <rev_count, timestamp, power_value>
isEOF = 0;
pos = 1;
i = 0;
mdata = []; //declare an empty matrix 
while isEOF <> 1
    [n, rev, ts] = mfscanf(fd,"%lf %lf");
    if n > 0 then
        mdata(pos,1) = rev;
        mdata(pos,2) = ts * 0.001;
        pos = pos + 1;
        i = i + StepValue;
    else
        isEOF = 1;
    end
end
mclose(fd);

//Sanitize the timestamp to start from 0
if mdata(1,2) <> 0 then
    ts = mdata(1,2);
    for i=1:length(mdata(:,2))
        mdata(i,2) = mdata(i,2) - ts;
    end
end

//Average values with the same timestamp (if exists)
avg =  [];
t = [];

index = 1;
i = 1;
while i < length(mdata(:,1)) - 1 
    tv = mdata(i,1);
    ts = mdata(i,2);
    j = i+1;
    while mdata(j,2) == ts
        if j == length(mdata(:,1)) then
            break;
        else
            tv = tv + mdata(j,1);
            j = j+1;
        end
         
    end
    tv = tv /(j-i);
    avg(index)=tv;
    t(index)=ts;
    index = index+1;
    i = j;
end
//
//plot the data
hf = scf(1);
clf(hf,'clear');
plot(t,avg,'b');
xs2png(hf, imgname+'_original.png');

fdata = avg;

//plot filtered data
 hf = scf(1);
 plot(t,fdata,'r--');
 xs2png(hf, imgname+'_orig+filtered.png');

//Tachometer estimation
y = zeros(1,length(t));
MaxCount =20;//round(length(t)/10);
//MaxCount = 1;
for i=2:length(fdata)
    if i <= MaxCount
        DeltaT = t(i) - t(1);
        
        y(i-1) = (fdata(i) - fdata(1))/DeltaT*%pi/180;
    else
        DeltaT = t(i) - t(i-MaxCount);
       
        y(i-1) = (fdata(i) - fdata(i-MaxCount))/DeltaT*%pi/180;
    end
end
y($) = y($-1);
hf = scf(2);
clf(hf);
plot(t, y, 'm');

/////////////////////////////////////////////////////////
//filter
[pols, gain] = zpbutt(3,10); //Filter
H = gain/(real(poly(pols,'s'))); 
Tc = 0.002;
H_C = ss2tf(dscr(H,Tc));
filt = filter(H_C.num,H_C.den,y);
plot(t,filt,'r');
y = filt;

////////////////////////////////////////////////////////

//estimate final value, 
MaxY = max(y);
fv_est = y($);

//Overshoot
OverShoot =abs(MaxY - fv_est)/abs(y(1)-fv_est);

//xi est
xi_est = sqrt((log(OverShoot))^2/(%pi^2 + (log(OverShoot))^2));

//Estimate the settling time
YMTs = fv_est*(1 + sAlpha);
YmTs = fv_est*(1 - sAlpha);
_index = 1;
for i=length(y):-1:1
    if y(i) > YMTs | y(i) < YmTs
        _index=i;
        break;
    end
end

st_est = t(_index) + StepValue/2;
q_n = st_est;

//omega_est
Nb = 1/sqrt(1 - xi_est^2);
omega_est = (log(sAlpha) - log(Nb))/(-xi_est*st_est);
////////////////////////////////////////////////////////

//estimate final value, 
MaxY = max(y);
fv_est = y($);

//Overshoot
OverShoot =abs(MaxY - fv_est)/abs(y(1)-fv_est);

//xi est
xi_est = sqrt((log(OverShoot))^2/(%pi^2 + (log(OverShoot))^2));

//Estimate the settling time
YMTs = fv_est*(1 + sAlpha);
YmTs = fv_est*(1 - sAlpha);
_index = 1;
for i=length(y):-1:1
    if y(i) > YMTs | y(i) < YmTs
        _index=i;
        break;
    end
end

st_est = t(_index) + StepValue/2;
q_n = st_est;

//omega_est
Nb = 1/sqrt(1 - xi_est^2);
omega_est = (log(sAlpha) - log(Nb))/(-xi_est*st_est);
//Estimate the Response using the model parameters
v_est =zeros(1,length(t));
StepTime = 1;
q_est = y($);
N = q_est/(2*sqrt(1 - xi_est^2));
phi = atan(xi_est, -sqrt(1 - xi_est^2));
y_est = q_est + 2*N*exp(-xi_n*omega_est*t).*cos(omega_est*sqrt(1 - xi_n^2)*t + phi);
v_est = y_est(1:$)';
hf = scf(2);
plot(t, y_est, 'g');
xs2png(hf, imgname+'_performance.png');
//calculate the perfomance indices
ISE = sum((v_est - y)^2)
IAE = sum(abs(v_est - y))
ITSE = sum(t'.*(v_est - y)^2)
ITAE = sum(t'.*abs(v_est - y))



