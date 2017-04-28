--require "lowerCPU"
require 'helper1'
require "acp"
require "delusion"
require "timer"
require "affinity"


DAd = 1
Ad  = 2
Js  = 3
Am  = 4

function startC()
  startCritical()
end

function stopC()
  stopCritical()
end

function invite()
  activateWin(Js) 
  delay(100) 
  pressFx(Js,9)
  
  activateWin(DAd)
end

function Cov()
 startC()
  activateWin(Js)  
  pressFx(Js,10)
  delay(100)
  activateWin(DAd)
 stopC()
end

function Dance()
  startC()
  activateWin(Ad)  
  pressFx(Ad,9)
  delay(100)
  activateWin(DAd)
  stopC()
end

function Stigma()
 startC()
  activateWin(Am)  
  pressFx(Am,2)
  delay(100)
  activateWin(DAd)
 stopC()
end

function Pov()
 startC()
  activateWin(Am)  
  pressFx(Am,11)
  delay(100)
  activateWin(DAd)
 stopC()
end


function Follow()
  startC()
  for win=2,getWinCount(),1 do
     activateWin(win)
     --delay(100)
--     if win == 2 then
  --     delay(150)
--     end
     for j=1,2,1 do
        pressEnter(win)
        typeString(win,"/target DarthAdriana")
        pressEnter(win)
        delay(50)        
     end
  end
  activateWin(1)
  stopC()  
end

registerHotKey("ctrl+z","invite","����� � ������ 1 ����")
registerHotKey("ctrl+N","Cov","��� ������ 3 �����")
registerHotKey("ctrl+B","Pov","Pov ������ 4 ����� ���������")
registerHotKey("ctrl+D","Dance","����� 2 �����")
registerHotKey("ctrl+s","Stigma","������ 4 �����")
registerHotKey("ctrl+F","Follow","��������� �� �������� ������")

