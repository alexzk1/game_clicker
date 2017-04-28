require "helper"
---------------------------настройки----------------------------------------
--перетащить прицел на полоску ЦП и показанные значения вписать сюда

acpX = 142
acpY = 28
acpC = 0x00206485

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
 end
end


math.randomseed(os.time())
registerHotKey("ctrl+F11","acp","Включить/выключить ACP")
