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

winX = 108
winY = 272
winC = 0x0000FD00

backX = 123
backY = 444

basebet = 1000
------------------------------------------------------------------------------------------------




button    = 1
lostchain = 0;

bet     = basebet
balance = 0

function doBet()  
  mouseLClick(1,fieldX,fieldY)
  delay(200)
  typeString(1,bet)   
  delay(500) 
  if button == 1 then
    mouseLClick(1,b1X,b1Y)
    --button = 2
  else
    mouseLClick(1,b2X,b2Y)
    --button = 1
  end
  for i=1,30,1 do
       textOut(1,24,81,string.format("Balance: %d",balance))
       delay(50)
  end

end


function play()
  while (getPressCount("play") % 2) == 1 do
    if isActiveWin(1) then
       balance = balance - bet
       doBet()
       local won = (getColor(1,winX,winY) == winC)
       if ( won ) then
          lostchain = 0
          balance = balance + bet + bet + bet
          for i=1,15,1 do
            textOut(1,24,81,string.format("Balance: %d",balance))
            delay(50)
          end

          if (balance > 0) then
             bet     = basebet
             balance = 0
          else
             local b = bet+bet
             bet = b
             --if (bet > (-1*balance)) then
              -- bet = (-1*balance)
             --end
             if (bet<1000) then
               bet = 1000
             end
          end

       else    --lost
          lostchain = lostchain+1
          if (1 == button) then
             button = 2
          else
             button = 1
          end
       end

       mouseLClick(1,backX,backY)
       if lostchain >6 then
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


function outt()
  showMsg(string.format("balance = %d",balance))
end

registerHotKey("ctrl+F12","play")
registerHotKey("ctrl+F11","outt")