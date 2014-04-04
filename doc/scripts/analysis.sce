//read input file containing the data
//format <rev_count, timestamp, power_value>
//Open and read file
fname = '../data/motor_30.dat';
LpAlpha = 0.9; //Alpha value for filter
DeltaT = 1;
N = 4; //Moving Average window
//Moving Average

    function [mvdata]=MovingAverage(mdata)
        dta = zeros(length(mdata));
        
    endfunction

fd = mopen(fname,'r');
if fd == -1 then
    error("Failed to open file for reading");
end

isEOF = 0;
pos = 1;
i = 0;
mdata = []; //declare an empty matrix
while isEOF <> 1
    [n, rev, ts, pv ] = mfscanf(fd,"%f %d %d");
    if n > 0 then
        mdata(pos,1) = rev;
        mdata(pos,2) = i; //change i to ts to get the real timestamp from the lego
        mdata(pos,3) = pv;
        pos = pos + 1;
        i = i + 1;
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

//Apply Exponential LowPassFilter
fdata = zeros(avg);
fdata(1) = avg(1);
 for i=2:length(avg)
     fdata(i)=LpAlpha*avg(i)+(1-LpAlpha)*avg(i-1);
 end
 
//plot filtered data
 hf = scf(1);
 plot(t,fdata,'r--');
 

//Tachometer estimation
y = zeros(1,length(t));
MaxCount = round(length(t)/2);
//MaxCount = 5;
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
