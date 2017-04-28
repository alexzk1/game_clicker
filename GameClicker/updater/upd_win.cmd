cd ..\winres\
copy ..\updater\release\gcupd.exe .\tmp.exe
call winres.cmd gcupd ..\updater\icons\update.ico "perl ..\get_sv_ver.pl" tmp.exe
del tmp.exe
move /Y gcupd.exe ..\win_release\
cd ..\updater\
