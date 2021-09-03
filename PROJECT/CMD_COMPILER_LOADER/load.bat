::---------------LOAD SETTINGS---------------
@echo off

del 1967VN044.sfr
del adc.bin

SET DIR_DEBUG=c:\Milandr\CM-LYNX.2.02.00
SET DXE_FILE=%1

SET DEBUGER=%DIR_DEBUG%\lldb-mi.exe
SET SETTINGS_FILE=setting.lldbinit
SET tmpfile=MYFILE.tmp

%DEBUGER% --source %SETTINGS_FILE%
exit

for /F "usebackq tokens=*" %%a in (`findstr /N "\<file.*" setting.lldbinit`) do (
    set "NUMBER_STR=%%~a"
)
set TMP=file
set seek=%NUMBER_STR:~3%

if exist %tmpfile% del /q %tmpfile%
for /f "delims=" %%a in (%SETTINGS_FILE%) do (
  (echo %%a)|>nul findstr "\<file.*"&&((echo file %DXE_FILE%)>>%tmpfile%)
  (echo %%a)|>nul findstr "\<file.*"||(echo %%a)>>%tmpfile%
)
call copy /y %tmpfile% %SETTINGS_FILE% >nul
del /f /q %tmpfile% >nul

timeout /t 5

%DEBUGER% --source %SETTINGS_FILE%