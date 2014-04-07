//clear workspace
clear;

// global variables
global fname; // used by analysis.sce
global imgname;
global StepValue;
global DATA_PATH;
DATA_PATH = '../data/';
global IMAGES_PATH;
IMAGES_PATH = '../images/';

global CURRENT_POWER; //Current power being analysed

//parameters
global st_est; //settling time
global omega_est; //omega_est
global OverShoot; //overshoot

dataFiles = ['10.dat','20.dat','30.dat','40.dat','50.dat','60.dat','70.dat','80.dat','90.dat','100.dat']; //list of data files

//get size
_ff = size(dataFiles);
fsize =_ff(2);

//"pass" the files to analysis.sce
Mfd = mopen('../data/params.dat', 'w');
if Mfd == -1 then
    error("Failed to open file for reading");
end

for i=1:fsize
    fname = DATA_PATH+dataFiles(i);
    imgname= IMAGES_PATH+dataFiles(i);
    exec('analysis.sce', -1);
    mfprintf(Mfd,"%d %f %f %f\n",CURRENT_POWER,st_est,omega_est,OverShoot);
end
mclose(Mfd);

//Plot the power values
fd =mopen('../data/params.dat','r');
if fd == -1 then
    error("Failed to open file for reading");
end

//read values from file
pData=[];
isEOF = 0;
pos = 1;
while isEOF <> 1
    [n, pwr, st, om,ov ] = mfscanf(fd,"%d %f %f %f");
    if n > 0 then
        pData(pos,1)=pwr;
        pData(pos,2)=st;
        pData(pos,3)=om;
        pData(pos,4)=ov;
        pos = pos + 1;
    else
        isEOF = 1;
    end
end

//plot power vs settling time
hf = scf(3);
clf(hf);
plot(pData(:,1),pData(:,2),'b'); 

//save plot
xs2png(hf, IMAGES_PATH+'_power_vs_st.png');


//plot power vs OverShoot
hf = scf(3);
clf(hf);
plot(pData(:,1),pData(:,4),'b');
xs2png(hf, IMAGES_PATH+'_power_vs_overshoot.png');

//plot power vs omega
hf = scf(3);
clf(hf);
plot(pData(:,1),pData(:,2),'b');
xs2png(hf, IMAGES_PATH+'_power_vs_omega.png');
