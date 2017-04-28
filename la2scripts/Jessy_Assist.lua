require "helper"

MainWin = 1
JessyWin = 2
AmielWin = 3


function JessyF4()
  activateWin(JessyWin)
  delay(100)
  pressFx(JessyWin,4)
--  if getWindowCount()>2 then
  --  activateWin(AmielWin)
   -- delay(100)
  --  pressFx(AmielWin,2)
  --end
  activateWin(MainWin)
end;

function JessyF7()
  activateWin(JessyWin)
  delay(100)
  pressFx(JessyWin,7)

  
  activateWin(MainWin)
end;


function Jessy8()
 activateWin(JessyWin)
 delay(100)
  sendMessage(JessyWin,0x0100,0x38,0); --WM_KEYDOWN
  sendMessage(JessyWin,0x0101,0x38,0); --WM_KEYUP
 activateWin(MainWin)
end;


registerHotKey("ctrl+q","JessyF4");
registerHotKey("ctrl+w","JessyF7");
registerHotKey("ctrl+s","Jessy8");
