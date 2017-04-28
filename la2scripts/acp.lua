require "helper1"
require "colors"

---------------------------настройки----------------------------------------
--перетащить прицел на полоску ЦП и показанные значения вписать сюда

acpX = 147
acpY = 29
acpC = rgb(136,90,0)

----------------------------------------------------------------------------



function randDelay()
 --случайная задержка (откат банки + антилог сервера)
 delay(200 + math.random(150,600))
 --большой случайный разброс, как от человека
end

function acp()
 while (getPressCount("acp") % 2) == 1 do
   if isActiveWin(1) then

      if getApiVersion()>100 then
         textOut(1,24,81,"ACP включено")
      end

     if math.random()<0.85 then  
     --шанс срабатывания 85% чтоб труднее отследить по логу сервера

        if getColor(1,acpX,acpY) ~= acpC then
           pressKx(1,3) --жму 3 панель 3
        end
     end --end of random
   end
   randDelay()
   --delay(1000)
 end
end


math.randomseed(os.time())
registerHotKey("ctrl+F10","acp","Включить/выключить ACP")

