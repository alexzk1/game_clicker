--снижает нагрузку на процессор окон, начина€ со 2го
--добавьте в ваш главный файл скрипта (без --)

--require "lowerCPU"

--дл€ использовани€

require "helper"
require "procs"

CPUHandles={}
CPUHandlesCount = 0


function CPU_winHandler(win)
  --убедимс€, что команда из другого скрипта попадет в активное окно, иначе все повиснет

  if (win>0) and (win<=CPUHandlesCount) then
   if (CPUHandles[win].handle) and (not CPUHandles[win].active) then
     startCritical()
         callDllA("NtResumeProcess","ntdll",CPUHandles[win].handle)
         CPUHandles[win].active = true
         CPUHandles[win].time = ticks()
         CPUHandles[win].lastaccess = ticks()
     stopCritical()
   end
  end
end


function CPUInit()
  CPUHandles={}
  CPUHandlesCount = getWinCount()
  for i=1,CPUHandlesCount,1 do
     CPUHandles[i] = {}
     CPUHandles[i].handle = handleByWin(i)
     if not CPUHandles[i].handle then
       showMsg(string.format("NULL handle for window %u",i))
     end
     CPUHandles[i].active = true
     CPUHandles[i].time   = ticks()     
     CPUHandles[i].hwnd   = getHWND(i)
     CPUHandles[i].lastaccess = 0
     delay(5)
  end
  addWindowsHandler("CPU_winHandler")
end

function CPUDone()
  if CPUHandlesCount > 0  then
   for i=1,CPUHandlesCount,1 do
      if (not CPUHandles[i].active) and (CPUHandles[i].handle) then
         callDllA("NtResumeProcess","ntdll",CPUHandles[i].handle)
         CPUHandles[i].active = true
      end
      if CPUHandles[i].handle then
        closeProcHandle(CPUHandles[i].handle)
        CPUHandles[i].handle = 0
      end      
   end
  end
  CPUHandlesCount = 0
end


function lowerCPUMain()
local work  = 70
CPUInit() 
local pause = work * CPUHandlesCount

 while (getPressCount("lowerCPUMain") % 2 == 1) and (CPUHandlesCount>1) do
    for i=1,CPUHandlesCount,1 do
      if CPUHandles[i].handle then



         if CPUHandles[i].active then
            if CPUHandles[i].time+work<ticks() then
               startCritical()
                 if not winIsActive(CPUHandles[i].hwnd) then

--останавливаем окно, если его не трогали 15000ms = 15 секунд
                   if CPUHandles[i].lastaccess+15000 < ticks() then
                       callDllA("NtSuspendProcess","ntdll",CPUHandles[i].handle)
                       CPUHandles[i].active = false
                   end

                 else
                   CPUHandles[i].lastaccess = ticks()
                 end
                 CPUHandles[i].time = ticks()
               stopCritical()
            end
         else
            if CPUHandles[i].time+pause<ticks() then
               startCritical()
                 callDllA("NtResumeProcess","ntdll",CPUHandles[i].handle)
                 CPUHandles[i].active = true
                 CPUHandles[i].time = ticks()
               stopCritical()
            end
         end     



      end
      delay(10)    
    end --for
    delay(1)
 end
 startCritical()
   CPUDone()
 stopCritical()
end

if getApiVersion() > 105 then
  registerHotKey("ctrl+alt+F12","lowerCPUMain","¬ключает/отключает снижение нагрузки на проц.")
  addStopHandler("CPUDone")
  emulateHotKey("ctrl+alt+F12")  
end
