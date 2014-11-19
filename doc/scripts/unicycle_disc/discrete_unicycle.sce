//Discrete

// Y[k] = (Y[k-1] - U[k-1]+A*U[k])*1/B
uni_GD = ss2tf(dscr(uni_C, Tc));
