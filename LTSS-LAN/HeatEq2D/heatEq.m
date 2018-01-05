function [Y logs] = heatEq(C, dx, T, dt, D, ROOM_TEMP)
  m = size(C, 1);
  n = size(C, 2);
  Y = C;
  logs = Y(:)';
  
  for t = 0.0:dt:T
    dC = FD(Y, dx, ROOM_TEMP);
    for i = 1:m
      for j = 1:n
        Y(i, j) = Y(i, j) + dt * D * dC(i, j);
      end
    end
    logs = [logs; Y(:)'];
  end
end

function dC = FD(C, dx, ROOM_TEMP)
  m = size(C, 1);
  n = size(C, 2);
  
  for i = 1:m
    for j = 1:n
      c = C(i, j);
      if i == 1
        u = ROOM_TEMP;
      else u = C(i-1, j);
      end
      
      if i == m
        d = ROOM_TEMP;
      else d = C(i+1, j);
      end
      
      if j == 1
        l = ROOM_TEMP;
      else l = C(i, j-1);
      end
      
      if j == n
        r = ROOM_TEMP;
      else r = C(i, j+1);
      end
      
      dC(i, j) = ( 1 / (dx * dx) ) * (u + d + l + r - 4 * c); 
    end
  end
end 
