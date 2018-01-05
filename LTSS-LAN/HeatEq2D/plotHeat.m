m = 20;
n = 20;
T = 2;
dt = 0.01;
dx = 0.1;
D = 0.1;
ROOM_TEMP = 25;
CENTER_TEMP = 80;

C0 = ones(m, n) * ROOM_TEMP;
C0(m/2-4 : m/2 + 5, n/2 - 4: n/2 + 5) = CENTER_TEMP;

[C, logs] = heatEq(C0, dx, T, dt, D, ROOM_TEMP);
disp(C);
X = 1:m;
Y = 1:n;
for i = 1:size(logs, 1)
	s = reshape(logs(i,:), [m n]);
	surf(s);
	axis([0 m 0 n 0 CENTER_TEMP + 10]);
  	axis manual
  	pause(0.02);
end
