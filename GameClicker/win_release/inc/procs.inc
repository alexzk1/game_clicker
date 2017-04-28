--вспомогательные функции для работы с процессами

function openProcHandle(pid)
  --requesting PROCESS_ALL_ACCESS
  return callDllA("OpenProcess","kernel32",0xF0000+0x100000+0xFFF,0,pid)
end

function closeProcHandle(handle)
  if handle then
      callDllA("CloseHandle","kernel32",handle)
  end
end

--получить указатель на процесс по номеру окна
--указатель должен быть закрыт вызовом closeProcHandle

function handleByWin(WinNum)
   local hwnd = getHWND(WinNum)

   local pPid = callDllA("VirtualAlloc","kernel32",0,4,0x3000,0x04)
   local pid  = 0

   if pPid then
      callDllA("GetWindowThreadProcessId","user32",hwnd,pPid)
      pid = derefAsDWORD(pPid)
      callDllA("VirtualFree","kernel32",pPid,0,0x8000)
      return openProcHandle(pid)
   else
     showMsg("VirtualAlloc failed")
   end
   return 0
end

function ticks()
   return callDllA("GetTickCount","kernel32")
end

function winIsActive(hwnd)
   return callDllA("GetForegroundWindow","user32")==hwnd
end
