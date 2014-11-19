clear;
//Get Average of values

//Open and read file
//names = ['../data/motor_100.dat','../data/motor_100n.dat','../data/motor_100nn.dat','../data/motor_100nnn.dat','../data/motor_100nnnn.dat'];
names = ['../data/verification/motor_100.dat','../data/verification/motor_100n.dat','../data/verification/motor_100nn.dat','../data/verification/motor_100nnn.dat','../data/verification/motor_100nnnn.dat'];
outname = '../data/verification/100.dat';
m = 1;
mavg = [];
isFirst = 1;
StepValue = 0.01;
_ff = size(names);
fsize = _ff(2);

while m <= fsize
fname = names(m);
fd = mopen(fname,'r');
if fd == -1 then
    error("Failed to open file for reading");
end
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

if isFirst == 1 then
    isFirst = 0;
    mavg=mdata;
else
    mavg = mavg + mdata;
end

m = m + 1;
end

mavg = mavg / fsize;

fd = mopen(outname,'w');

for i=1:length(mavg(:,1));
    mfprintf(fd,"%f %f %d\n",mavg(i,1),mavg(i,2),mavg(i,3));
end
mclose(fd);
clear;
