lrelease GameClicker.pro
copy *.qm .\win_release

call gc_win.cmd 
copy inc\*.inc .\win_release\inc\
copy inc\*.lua .\win_release\

cd updater
call upd_win.cmd
cd ..

perl update_dlls.pl
:exit
