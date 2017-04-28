require "helper"
function AmielF2()
  activateWin(AmielWin)
  delay(100)
  pressFx(AmielWin,2)
--  if getWindowCount()>2 then
  --  activateWin(AmielWin)
   -- delay(100)
  --  pressFx(AmielWin,2)
  --end
  activateWin(MainWin)
end;

function AmielMana()
  activateWin(AmielWin)
  delay(100)
  pressFx(AmielWin,1)
--  if getWindowCount()>2 then
  --  activateWin(AmielWin)
   -- delay(100)
  --  pressFx(AmielWin,2)
  --end
  activateWin(MainWin)
end

registerHotKey("ctrl+q","AmielF2");
registerHotKey("ctrl+e","AmielMana");