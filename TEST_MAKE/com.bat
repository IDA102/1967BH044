::Копилятор+Ассемблер+Компоновка
SET PATH_EXE=c:\Milandr\CM-LYNX.2.02.00\toolchain\Bin\make.exe 
SET PATH_MAKE=w:\MILANDR\MSO\CURRENT_FOLDER\TEST_MAKE\makefile
call %PATH_EXE% -f %PATH_MAKE%
call %PATH_EXE% -f %PATH_MAKE% clean