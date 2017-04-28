--вспомогательные функции для решения частых задач
--(c) 2011-2012 Alexex Z.
--файл может обновляеться, не вносите сюда изменений


function calcLParam(vkey)
  local  amap = callDllA("MapVirtualKeyA","user32",vkey,0);
  local  lP   = 1 + amap*65536;
  return lP
end

function checkIfRespond(win)
   r,e = callDllA("SendMessageTimeoutA","user32",getHWND(win),0,0,0,0x02,5,0)
   return (r~=0) --and (e~=1460) --ERROR_TIMEOUT
end

function delay(ms)
  coroutine.yield(ms)
end;

function pressEnter(win)
   sendMessage(win,0x0100,13,0); --WM_KEYDOWN
   sendMessage(win,0x0101,13,0); --WM_KEYUP
end;

function pressTab(win)
   sendMessage(win,0x0100,9,0); --WM_KEYDOWN
   sendMessage(win,0x0101,9,0); --WM_KEYUP
end;


function pressFx(win,num)
  local fn = 0x70+num-1
  sendMessage(win,0x0100,fn,0); --WM_KEYDOWN
  sendMessage(win,0x0101,fn,0); --WM_KEYUP
end;

function pressKx(win,num)
  local fn = 0x60+num
  sendMessage(win,0x0100,fn,0); --WM_KEYDOWN
  sendMessage(win,0x0101,fn,0); --WM_KEYUP
end

function typeString(win, str)
    for i=1,string.len(str),1 do
      local s = string.sub(str,i,i)
      sendChar(win,s)
    end
end


--все время забываю и путаю название
function msgBox(s)
  showMsg(s)
end