--из трех чисел делает 1 цвет
function rgb(r,g,b)
  return b*256*256+g*256+r
end

function rshiftb(val)
  return math.floor( (val - (val%256))/256)
end

--приблизительное сравнение двух цветов 0x00RRGGBB
--сравнивается каждый из 3 цветов на приблизительное равенство

function colorcmp(c1,c2)
   r1 = c1 % 256
   r2 = c2 % 256
   
   c1 = rshiftb(c1)
   c2 = rshiftb(c2)

   g1 = c1 % 256
   g2 = c2 % 256

   b1 = rshiftb(c1)
   b2 = rshiftb(c2)

   return ( math.abs(r1-r2)< 11) and ( math.abs(g1-g2)<11) and ( math.abs(b1-b2)< 11)
end