require "helper1"
require "rouletteI"

------------------------------------------------------------------------------------------------

basebet = 50000
------------------------------------------------------------------------------------------------




button    = 1
lostchain = 0;

bet     = basebet
balance = 0
prognose_len = 15

aprognose={}
cprognose = 0

function doBetRB(value, butt)  
  mouseLClick(1,rbFieldX,rbFieldY)
  delay(200)
  typeString(1,value)   
  delay(500) 
  if butt == 1 then
    mouseLClick(1,redX,redY)
  else
    mouseLClick(1,blackX,blackY)
  end
  delay(1500)
end

function prognose()
  local sum = 0.0;
  for i=1,prognose_len,1 do
      sum = sum + aprognose[i]
  end
  local avr = 0.0 + sum/prognose_len
  if (avr<-0.33) then
     return 1
  end
  if (avr>0.33) then 
     return 2
  end
  return math.random(2)
end

function addval(won,value)
  cprognose = cprognose + 1
  if (cprognose > prognose_len) then
    cprognose = 1
  end
  if not won then
     if value == 2 then
        value = 1
     else
        value = 2
     end
  end

  if value == 1 then
     aprognose[cprognose] = -1
  else
     aprognose[cprognose] =  1  
  end

end


function play()
 --три исхода: вероятнее красное (отриц.), равновероятно случайно, вероятнее черное (полож)
  for i=1,prognose_len,1 do
     aprognose[i] = 0
  end
  cprognose = 0
  button = 1

  while (getPressCount("play") % 2) == 1 do
    if isActiveWin(1) then
       
       doBetRB(bet,button)
       local won = (getColor(1,winX,winY) == winC)
       addval(won,button)

       if ( won ) then
          lostchain = 0
          balance = balance + bet

          if (balance < 0) then
             local b = bet+basebet
             bet = b
          else
             bet     = basebet
             balance = 0       
          end

       else    --lost
          balance = balance - bet
          lostchain = lostchain+1
       end       
       button = prognose()
       
       mouseLClick(1,backX,backY)
       for i=1,30,1 do
            textOut(1,24,81,string.format("Balance: %d",balance))
            delay(50)
       end

    end --if
  end --while
end--function


math.randomseed(os.time())
registerHotKey("ctrl+F11","play","Игра на красное/черное")
