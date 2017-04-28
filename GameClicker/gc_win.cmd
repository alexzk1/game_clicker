cd .\winres\
copy ..\release\GameClicker.exe .\tmp.exe
call winres.cmd GameClicker ..\images\main.ico "perl ..\get_sv_ver.pl" tmp.exe
del tmp.exe
move /Y GameClicker.exe ..\win_release\
cd ..\
