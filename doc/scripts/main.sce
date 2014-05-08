clear;
// global variables
global fname; // used by analysis.sce
global imgname;
global StepValue;
global DATA_PATH;
DATA_PATH = '../data/set_par/';
global IMAGES_PATH;
IMAGES_PATH = '../images/';

global CURRENT_POWER; //Current power being analysed

//parameters
global st_est; //settling time
global omega_est; //omega_est
global OverShoot; //overshoot
global xi_est;
global omega_n;
global xi_n;
data_prefix = 'log_power_'; //filename prefix
dataFiles = ['10','20','30','40','50','60','70','80','90','100']; //list of data files

//get size
_ff = size(dataFiles);
fsize =_ff(2);

//"pass" the files to analysis.sce
Mfd = mopen('../data/params.dat', 'w');
if Mfd == -1 then
    error("Failed to open params file for writing");
end

for i=1:fsize
    CURRENT_POWER = strtod(dataFiles(i));
    fname = DATA_PATH + data_prefix + dataFiles(i);
    imgname= IMAGES_PATH+dataFiles(i);
    exec('analysis.sce', -1);
    mfprintf(Mfd,"%d %f %f %f %f\n",CURRENT_POWER,xi_est,omega_est,st_est,OverShoot);
end
mclose(Mfd);

//Plot the power values
fd =mopen('../data/params.dat','r');
if fd == -1 then
    error("Failed to open file for reading");
end

//read values from file
pData= [];//zeros(fsize,5);
isEOF = 0;
pos = 1;
while isEOF <> 1
    [n, pwr, xi_e, om, st, ov ] = mfscanf(fd,"%d %f %f %f %f");
    if n > 0 then
        pData(pos,1)=pwr;
        pData(pos,4)=st;
        pData(pos,3)=om;
        pData(pos,5)=ov;
        pData(pos,2)=xi_e;
        pos = pos + 1;
    else
        isEOF = 1;
    end
end

//Compute the Least Square Squares Approximation for omega and xi
omega_n = sum(pData(:,3))/fsize;
xi_n = sum(pData(:,2))/fsize;


//plot power vs settling time
hf = scf(3);
clf(hf);
plot(pData(:,1),pData(:,4),'b'); 
xs2png(hf, IMAGES_PATH+'_power_vs_st.png');


//plot power vs xi
hf = scf(3);
clf(hf);
plot(pData(:,1),pData(:,2),'b');
xs2png(hf, IMAGES_PATH+'_power_vs_xi.png');


//plot power vs OverShoot
hf = scf(3);
clf(hf);
plot(pData(:,1),pData(:,5),'b');
xs2png(hf, IMAGES_PATH+'_power_vs_overshoot.png');

//plot power vs omega
hf = scf(3);
clf(hf);
plot(pData(:,1),pData(:,3),'b');
xs2png(hf, IMAGES_PATH+'_power_vs_omega.png');
