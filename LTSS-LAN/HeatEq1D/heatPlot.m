M = 20;
C = ones(1, M) * 25.0;
HEAD = 100;
dx = 0.1;
T = 1.0;
dt = 0.01;
D = 0.1;

x = 1:M;
y = 0:dt:T;
[last z] = heatEq1D(C, dx, T, dt, D, HEAD);
surf(x, y, z);
