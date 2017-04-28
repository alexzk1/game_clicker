require "helper1"
require "procs"

winHandle = 0

function abc()
 local cnt = 0
 pressFx(1,2)
end

registerHotKey("ctrl+1","abc")
