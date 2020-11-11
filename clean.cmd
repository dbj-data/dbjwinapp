@echo off
:: this is simple script but able 
:: to look into subfolders
:: thus be carefull ...
del /Q  /S  *.obj>NUL
del /Q  /S  *.ilk>NUL
del /Q  /S  *.pdb>NUL
del /Q  /S  *.filters>NUL
del /Q  /S  *.user>NUL

:: if any argument is given
:: *.exe and *.res are deleted too
IF [%1] == [] goto :eofscript

del /Q  /S  *.res>NUL
del /Q  /S  *.exe>NUL


:eofscript
