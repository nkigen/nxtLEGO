//used to verify the model params
global omega_n;
global xi_n;
global q_n;
global g_n; //
global DATA_PATH;
global fname;
DATA_PATH = '../data/';
global IMAGES_PATH;
IMAGES_PATH = '../images/';
global VER_PATH;
VER_PATH = DATA_PATH+'verification/';
global VER_IMG_PATH;
VER_IMG_PATH = IMAGES_PATH+'verification/';

verFiles = ['30.dat','80.dat','100.dat']; //list of data files

//get size
_ff = size(dataFiles);
fsize =_ff(2);

//Get the verification signal
   
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

//plot filtered data
 hf = scf(1);
 plot(t,fdata,'r--');
 xs2png(hf, imgname+'_orig+filtered.png');

//Tachometer estimation
y = zeros(1,length(t));
MaxCount = round(length(t)/2);
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

//Estimate the Response using the model parameters
StepTime = 1;
q_est = Y($);
N = q_est/(2*sqrt(1 - xi_n^2));
phi = atan(xi_n, -sqrt(1 - xi_n^2));
y_est = q_est + 2*N*exp(-xi_n*omega_n*t).*cos(omega_n*sqrt(1 - xi_n^2)*t + phi);
y_est = [zeros(1,length(0:StepValue:StepTime-StepValue)), y_est(1:$-length(0:StepValue:StepTime-StepValue))];

//calculate the perfomance indices



