require "helper1"

TimerWin = 1

function timerfunc()
   local stime = os.time()
   local tfhwnd  = getHWND(TimerWin)

   while (getPressCount("timerfunc") % 2) == 1 do
      if callDllA("GetForegroundWindow","user32")==tfhwnd then
         local td = os.difftime(os.time(),stime)
         local sr = ""
         
         if (td>0) and (td<120) then
            sr = string.format("(%02u:%02u)",(120-td)/60,(120-td)%60)
         end

         if (td>120) and (td<600) then
            sr = string.format("(%02u:%02u)",(600-td)/60,(600-td)%60)
         end

         if (td>600) and (td<1080) then
           if (td<960) then
              sr = string.format("(%02u:%02u)",(1080-td)/60,(1080-td)%60)
           else
              sr = string.format("(%02u:%02u) REBUFF",(1080-td)/60,(1080-td)%60)
           end
         end

         if (td>1080) and (td<1680) then
            sr = string.format("(%02u:%02u)",(1680-td)/60,(1680-td)%60)
         end

         local s = string.format("%02u:%02u   %s",td/60,td%60,sr)
         textOut(TimerWin,24,81,s)
      end
      delay(10)
   end
end



registerHotKey("ctrl+alt+T","timerfunc","Òàéìåğ çàìêîâîé ïàéëàêè")
