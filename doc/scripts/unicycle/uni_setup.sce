global uni_x;
global uni_y;
global uni_theta;
global v;
global uni_Tc;
global D;
global R;
R = 0.028;
D = 0.11;
uni_Tc = 0.004;
v = 4; //rad/s
uni_theta = 0.707;
uni_x = 0.7;
uni_y = 4.5;
q0=[0;2;%pi];
//Unicycle linear model

//G_uni = v/s^2;

//Init the motor parameters
exec('/home/nkigen/NXT/nxtLEGO/doc/scripts/unicycle/rootlocus.sce',-1);
exec('/home/nkigen/NXT/nxtLEGO/doc/scripts/unicycle/unicycle_model.sce',-1);
exec('/home/nkigen/NXT/nxtLEGO/doc/scripts/controller/rootlocus_ideal.sce',-1);
exec('/home/nkigen/NXT/nxtLEGO/doc/scripts/controller/rootlocus_encoder.sce',-1);
