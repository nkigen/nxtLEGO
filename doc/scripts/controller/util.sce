//Calculate the z transform
z = %z;

Tc = 0.002;
Kc = 40;
z1 = -3;
z2 = -20; 
p1 = -80; 

a = Tc*(z1+z2)+2;
b = Tc*(z1+z2) + z1*z2*(Tc^2) + 1;
x = (p1*Tc+2);
y = 1+Tc*p1;

U = (z^2 -z*a + b)/(z^2-x*z+y);
//U.den = z^2-x*z+y;
