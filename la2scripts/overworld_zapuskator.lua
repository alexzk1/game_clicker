require "helper"
require "inirdr"


function run_win()
   local config = getUserFolder(true).."\\owzapuscator.cfg"

   ini = NewIniReader()
   ini:Read(config)
   showMsg(ini:GetValue("PATH","la2path"))
end



registerHotKey("ctrl+R","run_win","Запуск клиента оверволд с перезаписыванием файлов.")