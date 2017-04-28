require "helper"
require "procs"

function getCPUCount()
--returns amount of processor in the system
   local pSysInfo = callDllA("VirtualAlloc","kernel32",0,32,0x3000,0x04)
   local cpus = 1

   if pSysInfo then
      callDllA("GetSystemInfo","kernel32",pSysInfo)
      cpus = derefAsDWORD(pSysInfo+20)
      callDllA("VirtualFree","kernel32",pSysInfo,0,0x8000)
   else
     showMsg("VirtualAlloc failed into getCPUCount()")
   end
   return cpus
end


function setAffinity()
  startCritical()
  local CPUHandlesCount = getWinCount()  
  local cpus = math.pow(2, getCPUCount()) - 1  --mask of all cpus
  if cpus then
    for i=1,CPUHandlesCount,1 do
       local hProc = handleByWin(i)
       if hProc then
          if i == 1 then 
            callDllA("SetProcessAffinityMask","kernel32",hProc,1)
          else
            callDllA("SetProcessAffinityMask","kernel32",hProc,cpus-1)
          end
          closeProcHandle(hProc)
       end
    end
  end
  stopCritical()
end

function resAffinity()
  startCritical()
  local CPUHandlesCount = getWinCount()  
  local cpus = math.pow(2, getCPUCount()) - 1  --mask of all cpus
  if cpus then
    for i=1,CPUHandlesCount,1 do
       local hProc = handleByWin(i)
       if hProc then
          callDllA("SetProcessAffinityMask","kernel32",hProc,cpus)
          closeProcHandle(hProc)
       end
    end
  end
  stopCritical()
end


if getApiVersion() > 106 then
  registerHotKey("ctrl+alt+C","setAffinity","Выставляет 1е окно на все ядра, остальные окна на все, кроме 0го ядра.")
  registerHotKey("ctrl+alt+R","resAffinity","Разрешает всем окнам все ядра (сброс).")
end
