require "helper"

function f1()
   while (true) do
     pressFx(1,1);
     delay(1000)
   end
end

registerHotKey("ctrl+s","f1")
