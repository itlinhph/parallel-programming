L = 3.2;
dx = 0.1;
T = 100;
dt = 0.1;
C = 0.1;

w = waveEq(L, dx, T, dt, C);
x = 0.1:dx:L+0.1;
y = 0:dt:T;

%surf(x, y, w);
nTimes = ceil(T/dt) + 1;
% figure(1)

for i = [1:nTimes]
  plot(x, w(i, :), 'linewidth', 2);
  axis([0.1 L+0.1 -1.1 1.1]);
  axis manual
  pause(0.02);
end
