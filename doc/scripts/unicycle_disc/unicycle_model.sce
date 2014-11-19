function out = Unicycle(in)
  
  // Inputs
  v = in(1);
  omega = in(2);
  x = in(3);
  y = in(4);
  theta = in(5);
  
  
  // Kinematic model
  x_dot = cos(theta)*v;
  y_dot = sin(theta)*v;
  theta_dot = omega;
  
  
  // Outputs
  out = [x_dot; y_dot; theta_dot];
  
endfunction
