global fname;// 
global imgname;
global DATA_PATH;
global IMAGES_PATH;

global CURRENT_POWER;
global st_est; //settling time
global xi_est;
global q_n;
global omega_est; //omega_est
global OverShoot; //overshoot

LpAlpha = 0.575; //Alpha value for filter
sAlpha = 0.1; //Settling time alpha (10%)
global StepValue;
StepValue = 0.01;  

N = 20; //Moving Average Window
function [fdata]=MovingAverage(mdata)
    dta=zeros(mdata);
    for i=1:N
        dta(i)=mdata(i);
    end
    for n=N+1:length(mdata)-N
        sum = 0;
        for k=n-N:n
            sum = mdata(n)+sum;
        end
        dta(n)=sum/N;
    end
    for n=length(mdata)-N+1:length(mdata)
        dta(n)=mdata(n);
    end
    fdata = dta;
endfunction

//Exponential Filter
    function [efdata] =ExponentialFilter(alph,avg)
        fdata = zeros(avg);
        fdata(1) = avg(1);
        for i=2:length(avg)
            fdata(i)=alph*avg(i)+(1-alph)*avg(i-1);
        end
        efdata=fdata;
    endfunction
    
//Open and read file
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
    [n, rev, ts, pv ] = mfscanf(fd,"%f %f %d");
    if n > 0 then
        mdata(pos,1) = rev;
        mdata(pos,2) = i; //change i to ts to get the real timestamp from the lego
        mdata(pos,3) = pv;
        pos = pos + 1;
        i = i + StepValue;
    else
        isEOF = 1;
    end
end
mclose(fd);

//Sanitize the timestamp to start from 0
CURRENT_POWER=mdata(1,3);
if mdata(1,2) <> 0 then
    ts = mdata(1,2);
    for i=1:length(mdata(:,2))
        mdata(i,2) = mdata(i,2) - ts;
    end
end

//Average values with the same timestamp
avg = [];
t=[];
index = 1;
i = 1;
while i < length(mdata(:,1)) - 1
    tv = mdata(i,1);
    ts = mdata(i,2);
    j = i+1;
    while mdata(j,2) == ts
        tv = tv + mdata(j,1);
        j = j+1; 
    end
    tv = tv /(j-i);
    avg(index)=tv;
    t(index)=ts;
    index = index+1;
    i = j;
end

//plot the data
hf = scf(1);
clf(hf,'clear');
plot(t,avg,'b');
xs2png(hf, imgname+'_original.png');

//Apply LowPassFilter
fdata=ExponentialFilter(LpAlpha,avg);
//fdata=MovingAverage(avg);
//plot filtered data
 hf = scf(1);
 plot(t,fdata,'r--');
 xs2png(hf, imgname+'_orig+filtered.png');

//Tachometer estimation
y = zeros(1,length(t));
MaxCount = round(length(t)/3);
//MaxCount = 50;
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

hf = scf(2);
plot([st_est,st_est],[min(y),max(y)],'g--');
plot(t, YMTs*ones(1,size(t,2)), 'r--');
plot(t, YmTs*ones(1,size(t,2)), 'r--');
xs2png(hf, imgname+'_tachometer.png');
