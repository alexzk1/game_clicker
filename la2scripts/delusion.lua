require "helper1"
require "colors"

------------настройки для окна после сброса для 1152х864-------------------

windowLeader = 1 --номер лидера группы-окна

------------войти в печать диалог------------------------------------------
enterX = 121
enterY1 = 298
enterY2 = enterY1 -3 

enterC = rgb(77,113,188)
---------------------------------------------------------------------------
changeRoomX = 124
changeRoomY1 = 285
changeRoomY2 = changeRoomY1 - 2

changeRoomC = enterC+0
---------------------------------------------------------------------------
exitSealX = 124
exitSealY2 = 311
exitSealY1 = 312
exitSealC = changeRoomC+0
---------------------------------------------------------------------------

--у лидера нужно 2 макроса:
--1:
--   /target Guardian of Western Seal
--   /attack

--2:
--     /target Delusion Manager
--   /attack
--ставим ети макросы на Ф1 и Ф2 на панель







function enterTheSeal()
  if isActiveWin(windowLeader) then
    local clr1 = getColor(windowLeader, enterX, enterY1)
    local clr2 = getColor(windowLeader, enterX, enterY2)
    if (colorcmp(clr1,enterC)) then
        mouseLClick(windowLeader,enterX,enterY1)
        return true
    end
    if (colorcmp(clr2,enterC)) then
        mouseLClick(windowLeader,enterX,enterY2)
        return true
    end
  end
  return false
end
---------------------------------------------------------------------------
function openSealEntry()
  if isActiveWin(windowLeader) then
      pressFx(windowLeader,1)
  end
end
---------------------------------------------------------------------------
function openManager()
   if isActiveWin(windowLeader) then
       pressFx(windowLeader,2)
   end
end
---------------------------------------------------------------------------
function changeRoom()
  if isActiveWin(windowLeader) then
    if (colorcmp(getColor(windowLeader, changeRoomX, changeRoomY1),changeRoomC)) then
        mouseLClick(windowLeader,changeRoomX,changeRoomY1)
        return true
    end
    if (colorcmp(getColor(windowLeader, changeRoomX, changeRoomY2),changeRoomC)) then
        mouseLClick(windowLeader,changeRoomX,changeRoomY2)
        return true
    end

  end
  return false
end
---------------------------------------------------------------------------
function exitSeal()
  if isActiveWin(windowLeader) then
    if (colorcmp(getColor(windowLeader, exitSealX, exitSealY1),exitSealC)) then
        mouseLClick(windowLeader,exitSealX,exitSealY1)
        return true
    end

    if (colorcmp(getColor(windowLeader, exitSealX, exitSealY2),exitSealC)) then
        mouseLClick(windowLeader,exitSealX,exitSealY1)
        return true
    end

  end
  return false
end
---------------------------------------------------------------------------

function dummy()
  while (true) do
      delay(10)
  end
end

function main()   

  while (getPressCount("main") % 2) == 1 do
   delay(1000)
   while not enterTheSeal() do
     if (getPressCount("main") % 2) ~= 1 then
       return
     end
     delay(1000)
     openSealEntry()
     delay(1000)
   end

   delay(10000)
   while not changeRoom() do
     if (getPressCount("main") % 2) ~= 1 then
       return
     end
     openManager()
     delay(1000)
   end

   delay(10000)
   while not exitSeal() do
     if (getPressCount("main") % 2) ~= 1 then
       return
     end
     openManager()
     delay(1000)
   end
   delay(4000)
 end
end


---------------------------------------------------------------------------
--activateWin(1)
--showMsg(string.format("%X",getColor(windowLeader, changeRoomX, changeRoomY)))

if getApiVersion()<103 then
   showMsg("Обновите программу! Проход по печатям не активен.")
else
   registerHotKey("ctrl+space","main","Вход в печати")
end