d=100
f=load('result2.txt');
m=1:100;
n=1:100;
[m n] = meshgrid(m,n);
for i=1:10000
  d = f(i*100+1:(i+1)*100,1:100);
  index = find(d==0);
  d(index) = 0.3; % chon mau vi khuan
  imagesc(d);
  axis square;
  lm = colormap(hsv);
  colorbar;
  pause(0.04);
end 
