//read input file containing the data
//format <rev_count, timestamp, power_value>
//Open and read file
fname = '../data/motor_70.dat';
LpAlpha = 0.35; //Alpha value for filter
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
        mdata(pos,2) = i;
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
plot(t,avg);

//Apply Exponential LowPassFilter
fdata = zeros(avg);
fdata(1) = avg(1);
 for i=2:length(avg)
     fdata(i)=LpAlpha*avg(i)+(1-LpAlpha)*avg(i-1);
 end
//plot filtered data
 hf = scf(1);
 plot(t,fdata,'r');
 
//Tachometer estimation

//convert to radians
//fdata = fdata.*(%pi/180);

//Derivative
len = length(fdata);
deriv = zeros(len);
for i=2:len
    deriv(i-1) = (fdata(i)- fdata(i-1))/((t(i)-t(i-1)));
end
deriv(len)=deriv($);

hf = scf(2);
plot(t,deriv,'b');

