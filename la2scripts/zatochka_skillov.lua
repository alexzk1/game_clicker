require "helper1"


function tochi()
  while getPressCount("tochi")%2 == 1 do
     if isActiveWin(1) then
          --mouseLClick(3,297,417)
          mouseLClick(1,342,417)
          delay(200)
          mouseLClick(1,341,412)
          --mouseLClick(3,297,412)
          delay(500)
          mouseLClick(1,541,643);
          delay(700)
          mouseLClick(1,595,429);
          delay(2000)
     end
  end
end

registerHotKey("space","tochi")