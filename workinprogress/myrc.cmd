@echo off
::
:: there are ocassions when rc does not know about otherwise
:: well known codes etc.
:: usually when rc file is made by hand 
:: and/or winres.h is not included
:: in that case it is very advisable to tell to
:: the rc where is windows.h
:: and even include <windows.h> in the problem 
:: *.rc
:: of course change this to your local location
@set "winloc=D:\Windows Kits\10\Include\10.0.18362.0\um"
@rc /nologo /i"%winloc%" %1
@set "winloc="
