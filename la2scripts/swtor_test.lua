require "helper1"

function tst()
 while (getPressCount("acp") % 2) == 1 do
   if isActiveWin(1) then

      if getApiVersion()>100 then
         textOut(1,24,81,"ACP включено")
      end

   end
 end
end


math.randomseed(os.time())
registerHotKey("ctrl+F11","tst","¬ключить/выключить ACP")
