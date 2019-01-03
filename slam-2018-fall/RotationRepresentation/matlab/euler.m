clc; clear

alpha = pi/6; beta = pi/6; gamma = pi/3;
rot3x = [1 0 0; 0 cos(alpha) sin(alpha); 0 -sin(alpha) cos(alpha)]
rot3y = [cos(beta) 0 -sin(beta); 0 1 0; sin(beta) 0 cos(beta)]
rot3z = [cos(gamma) sin(gamma) 0; -sin(gamma) cos(gamma) 0; 0 0 1]
% rot3 = rot3z*rot3y*rot3x
rot3 = rot3y*rot3z
[v, d] = eig(rot3)

rotaxis = [rot3(2,3) - rot3(3,2); rot3(3,1) - rot3(1,3); rot3(1,2) - rot3(2,1)]
rotaxis = rotaxis/sqrt(rotaxis'*rotaxis)
