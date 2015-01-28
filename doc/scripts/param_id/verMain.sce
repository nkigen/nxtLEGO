//main file for verifying the model
global DATA_PATH;
global CURRENT_POWER; //Current power being ana
global fname; //prefix to be used for the current file to be analysed by verification.sce
global imgname;
//performance indices
global ISE;
global IAE;
global ITSE;
global ITAE;
global CURRENT_POWER; //Current power being analysed 
DATA_PATH = '../../data/data_left/param_id/';
global IMAGES_PATH;
IMAGES_PATH = '../../images/left_motor/';
global VER_PATH;
VER_PATH = DATA_PATH+'verification/';
global VER_IMG_PATH;
VER_IMG_PATH = IMAGES_PATH+'verification/';

verFiles = ['power_30.data','power_80.data','power_100.data']; //list of data files
pow__ = [30,80,100];
//get size
_ff = size(verFiles);
fsize =_ff(2);

//Open file to store the verification details
Vfd = mopen(VER_PATH+'performance.dat', 'w');
if Vfd == -1 then
    error("Failed to open file for reading");
end

for i = 1:fsize
    fname = DATA_PATH+verFiles(i);
    imgname= VER_IMG_PATH+verFiles(i);
    CURRENT_POWER=pow__(i);
    exec('verification.sce', -1);
    mfprintf(Vfd,"%d %f %f %f %f\n",CURRENT_POWER, ISE, IAE, ITSE, ITAE);
end
mclose(Vfd);
