require "helper1"
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
        textOut(1,24,81,"ACP включено")
        if getColor(1,acpX,acpY) ~= acpC then
           pressKx(1,3) --жму 3 панель 3
        end
   end
   if isActiveWin(2) then
        textOut(2,24,81,"ACP включено")
        if getColor(2,acpX,acpY) ~= acpC then
           pressKx(2,3) --жму 3 панель 3
        end
   end
   randDelay()
 end
end


math.randomseed(os.time())
registerHotKey("ctrl+F11","acp","Включить/выключить ACP")
