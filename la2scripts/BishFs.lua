require "helper"

FsWin   = 1
BishWin = 2

function Cleanse()
   activateWin(BishWin)
   delay(100)
   pressFx(BishWin,1)
   activateWin(FsWin)
   pressFx(FsWin,2)
end

function Stigma()
   activateWin(BishWin)
   delay(100)
   pressFx(BishWin,2)
   activateWin(FsWin)
end

registerHotKey("ctrl+Q","Cleanse")
registerHotKey("ctrl+S","Stigma")