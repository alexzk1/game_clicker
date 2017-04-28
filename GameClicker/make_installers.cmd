call make_wrelease.cmd

rem makensis /DEXE="SmartViewer.exe" /DLNK="SmartViewer.lnk" VScript.nsi
for /F %%V in ('getver.exe .\win_release\GameClicker.exe') do makensis  /DVER="%%V" GameClicker.nsi