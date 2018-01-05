function W = waveEq(L, dx, T, dt, C)
  nTimes = ceil(T / dt) + 1;
  nParts = ceil(L / dx) + 1;
  r = (C * dt / dx) ^ 2;
  
  W = zeros(nTimes, nParts); 
  X = 0:dx:L;
  %X = 0:nParts-1;
  W(1, :) = sin(2 * pi * X / L);
  %W(1, :) = sin(2 * pi * X / 8);
  
  %dW = spaceDis(W(1, :), nParts);
  %for i = 1:nParts
  %  W(2, i) = W(1, i) + 0.5 * r * dW(i);
  %end
  W(2, :) = W(1, :);
  
  for i = 3:nTimes
    dW = spaceDis(W(i-1, :), nParts);
    for j = 1:nParts
      W(i, j) = 2 * W(i-1, j) - W(i-2, j) + r * dW(j);
    end
  end
end

function dW = spaceDis(W, nParts)
  for i = 1:nParts
    if i == 1
      u = -W(2);
    else u = W(i-1);
    end
    if i == nParts
      v = -W(nParts - 1);
    else v = W(i+1);
    end
    dW(i) = u - 2 * W(i) + v;
  end
end
