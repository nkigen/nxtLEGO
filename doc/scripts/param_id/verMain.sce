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
DATA_PATH = '../data/';
global IMAGES_PATH;
IMAGES_PATH = '../images/';
global VER_PATH;
VER_PATH = DATA_PATH+'verification/';
global VER_IMG_PATH;
VER_IMG_PATH = IMAGES_PATH+'verification/';

verFiles = ['10.dat','20.dat','30.dat','40.dat','50.dat','60.dat','70.dat','80.dat','90.dat','100.dat']; //list of data files

//get size
_ff = size(dataFiles);
fsize =_ff(2);

//Open file to store the verification details
Vfd = mopen(VER_PATH+'performance.dat', 'w');
if Vfd == -1 then
    error("Failed to open file for reading");
end

for i = 1:fsize
    fname = VER_PATH+verFiles(i);
    imgname= VER_IMG_PATH+dataFiles(i);
    exec('verification.sce', -1);
    mfprintf(Vfd,"%d %f %f %f %f\n",CURRENT_POWER, ISE, IAE, ITSE, ITAE);
end
mclose(Vfd);

