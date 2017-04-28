--снижает нагрузку на процессор окон, начина€ со 2го
--добавьте в ваш главный файл скрипта (без --)

--require "lowerCPU"

--дл€ использовани€

require "helper"
require "procs"

CPUHandles={}
CPUHandlesCount = 0
CPUUsedHwnd={}


function CPU_winHandler(win1)
  --убедимс€, что команда из другого скрипта попадет в активное окно, иначе все повиснет
  if (win1>0) and (win1<=CPUHandlesCount) then  
   local hw  = CPUHandles[win1].hwnd
   local win = CPUUsedHwnd[hw]

   if (win>0) and (win<=CPUHandlesCount) then  
     if (CPUHandles[win].handle) and (not CPUHandles[win].active) then     
         callDllA("NtResumeProcess","ntdll",CPUHandles[win].handle)
         CPUHandles[win].active = true
         CPUHandles[win].time = ticks()
         CPUHandles[win].lastaccess = ticks()
     end
   end
  end
end


function CPUInit()
  startCritical()
  CPUHandles={}
  CPUUsedHwnd={}
  CPUHandlesCount = getWinCount()  
  for i=1,CPUHandlesCount,1 do
     CPUHandles[i] = {}
     local hwnd = getHWND(i)
     CPUHandles[i].active = true
     CPUHandles[i].time   = ticks()     
     CPUHandles[i].hwnd   = hwnd
     CPUHandles[i].lastaccess = 0

     if CPUUsedHwnd[hwnd]==nil then
       CPUHandles[i].handle = handleByWin(i)
       CPUUsedHwnd[hwnd] = i
     else
       --этот номер окна дублирующий
       CPUHandles[i].handle = 0;
     end     

     delay(5)
  end
  stopCritical()
  addWindowsHandler("CPU_winHandler")
end

function CPUDone()
  rmWindowsHandler("CPU_winHandler")
  if CPUHandlesCount > 0  then
   for i=1,CPUHandlesCount,1 do
      startCritical()
      if (not CPUHandles[i].active) and (CPUHandles[i].handle~=0) then        
           callDllA("NtResumeProcess","ntdll",CPUHandles[i].handle)       
           CPUHandles[i].active = true
           closeProcHandle(CPUHandles[i].handle)
           CPUHandles[i].handle = 0        
      end
      stopCritical()
   end
  end
  CPUHandlesCount = 0
  CPUHandles={}
  CPUUsedHwnd={}
end


function lowerCPUMain()
local work  = 60
CPUInit() 
local pause = work * CPUHandlesCount

 while (getPressCount("lowerCPUMain") % 2 == 1) and (CPUHandlesCount>0) do
    delay(1)
    for i=1,CPUHandlesCount,1 do
      if CPUHandles[i].handle and CPUHandles[i].hwnd then
         if CPUHandles[i].active then
            if CPUHandles[i].time+work<ticks() then               
              startCritical()
                 if not winIsActive(CPUHandles[i].hwnd) then
                   --останавливаем окно, если его не трогали 15000ms = 15 секунд                   
                   if CPUHandles[i].lastaccess+10000 < ticks() then
                     
                       callDllA("NtSuspendProcess","ntdll",CPUHandles[i].handle)
                       CPUHandles[i].active = false
                       CPUHandles[i].time = ticks()                     
                   end
                 else
                   CPUHandles[i].lastaccess = ticks()
                 end
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
      delay(1)
    end --for
 end
 CPUDone()
end

if getApiVersion() > 106 then
  registerHotKey("ctrl+alt+F12","lowerCPUMain","¬ключает/отключает снижение нагрузки на проц.")
  addStopHandler("CPUDone")
  emulateHotKey("ctrl+alt+F12")    
end
