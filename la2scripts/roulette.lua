require "helper1"


------------------------------------------------------------------------------------------------
------настройки для окна по умолчанию при 1152х864
fieldX = 123
fieldY = 345

b1X = 243
b1Y = 344

b2X = 299
b2Y = 344

--winX = 112
--winY = 270
--winC = 0x0003E404

winX = 111
winY = 272
winC = 0x0001FC01

backX = 123
backY = 444

basebet = 1000
------------------------------------------------------------------------------------------------



firstbet  = 0
secondbet = basebet
betcntr   = 1

button    = 1

won = true
lostchain = 0;

function nextBet()  
  local f = secondbet
  if getApiVersion()>101 then 
  if betcntr>2 then
    secondbet = firstbet+secondbet
  end
  betcntr=betcntr+1
  firstbet = f;

  mouseLClick(1,fieldX,fieldY)
  delay(200)
  typeString(1,secondbet)   
  delay(500) 
  if button == 1 then
    mouseLClick(1,b1X,b1Y)
    button = 2
  else
    mouseLClick(1,b2X,b2Y)
    button = 1
  end
  delay(1000)
  end
end

function newBet()

  firstbet  = 0
  secondbet = basebet
  betcntr=1
  nextBet()
end

function play()
  while (getPressCount("play") % 2) == 1 do
    if isActiveWin(1) then
       if won then
         newBet()
       else
         nextBet()
       end
       won = (winC == getColor(1,winX,winY) )
       if (won) then
          lostchain = 0
       else
          lostchain = lostchain+1
       end
       mouseLClick(1,backX,backY)

       if lostchain >8 then
          delay(1000)
          pressFx(1,9)
          lostchain = 1 
          delay(60000)
       else
          delay(1000)  
       end
    end
  end
end

--registerHotKey("n","newBet");
--registerHotKey("space","nextBet");

registerHotKey("ctrl+F12","play")
