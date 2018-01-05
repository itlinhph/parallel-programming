function [Y logs] = heatEq1D(C, dx, T, dt, D, HEAD)
  m = length(C);
  Y = C;
  logs = [];
  TAIL = C(length(C));
  
  for t = 0.0:dt:T
    logs = [logs; Y];
    dC = FD(Y, dx, HEAD, TAIL);
    for i = 1:m
      Y(i) = Y(i) + dt * D * dC(i);
    end
  end
end

function dC = FD(C, dx, head, tail)
  m = length(C);
  
  for i = 1:m
    c = C(i);
    if i == 1
      u = head;
    else u = C(i-1);
    end
    
    if i == m
      d = tail;
    else d = C(i+1);
    end
    
    dC(i) = ( 1 / (dx * dx) ) * (u + d - 2 * c); 
  end
end 
