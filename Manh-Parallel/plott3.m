m = 20
n = 20
high_temp = 250
c = load('laplace.txt');
a = 1:m;
b = 1:n;
[A, B] = meshgrid(a, b);
axis equal;
for i = 0:1000000000
    D = c(i*m+1:(i+1)*m, 1:n);
    surf(A, B, D);
    axis([0 m+1 0 n+1 0 high_temp]);
    axis manual;
    pause(0.04);
end;
