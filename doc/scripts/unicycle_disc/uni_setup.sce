global uni_x;
global uni_y;
global y_ref;
global uni_theta;
global v;
global uni_Tc;
global Tc;
global Kc_uni;
global D;
global R;
global w_sat_min;
global w_sat_max;
z =%z;

y_ref = 0.5;
w_sat_min = -10;
w_sat_max = 10;
Tc = 0.001;
R = 0.028;
D = 0.11;
Kc_uni = 0.001;
uni_Tc = 0.004;
v = 8; //rad/s
uni_theta = 0.707;
uni_x = 0.7;
uni_y = 4.5;

//uni_c = (z+1)/(z+2); //test
//Unicycle linear model

//G_uni = v/s^2;

//Init the motor parameters
exec('/home/nkigen/NXT/nxtLEGO/doc/scripts/controller/rootlocus_ideal.sce',-1);
exec('/home/nkigen/NXT/nxtLEGO/doc/scripts/controller/rootlocus_encoder.sce',-1);
exec('/home/nkigen/NXT/nxtLEGO/doc/scripts/controller/discrete.sce',-1);
exec('/home/nkigen/NXT/nxtLEGO/doc/scripts/unicycle_disc/rootlocus.sce',-1);
exec('/home/nkigen/NXT/nxtLEGO/doc/scripts/unicycle_disc/unicycle_model.sce',-1);
exec('/home/nkigen/NXT/nxtLEGO/doc/scripts/unicycle_disc/discrete_unicycle.sce',-1);
