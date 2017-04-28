require "helper"

DAdrian = 1
Adrian  = 2
Amiel   = 3

function gopets()
   while (true) do
     activateWin(Adrian)
     delay(100)
     pressFx(Adrian,2);
     pressFx(Adrian,6);
     pressFx(Adrian,4);
     activateWin(DAdrian)
     pressFx(DAdrian,2);
     delay(12000)
   end
end

function stigma()
     activateWin(Amiel)
     delay(100)
     pressFx(Amiel,2)
     activateWin(DAdrian)
end


registerHotKey("ctrl+S","gopets")
registerHotKey("ctrl+Q","stigma")