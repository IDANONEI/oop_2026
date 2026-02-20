@echo off

set PROGRAM="%~1"
set OUT="%TEMP%\out.txt"

::infile - OK по тз
%PROGRAM% < test1.txt > %OUT% || goto err
fc %OUT% test1-out.txt || goto err

::infile - OK по тз
%PROGRAM% < test2.txt > %OUT% || goto err
fc %OUT% test2-out.txt || goto err

::Non-invertible
%PROGRAM% < test3.txt > %OUT% || goto err
fc %OUT% test3-out.txt || goto err

::Invalid matrix - буквы
%PROGRAM% < test4.txt > %OUT% 
fc %OUT% test4-out.txt || goto err

::Invalid matrix 1: 2 числа
%PROGRAM% < test5.txt > %OUT% 
fc %OUT% Invalidmatrixf.txt || goto err

::Invalid matrix пустой
%PROGRAM% < test6.txt > %OUT% 
fc %OUT% Invalidmatrixf.txt || goto err

::2 строки
%PROGRAM% < test7.txt > %OUT% 
fc %OUT% Invalidmatrixf.txt || goto err

::4 числа
%PROGRAM% < test8.txt > %OUT% 
fc %OUT% Invalidmatrixf.txt || goto err

::infile - OK по тз
%PROGRAM% test1.txt > %OUT% || goto err
fc %OUT% test1-out.txt || goto err

::help
%PROGRAM% -h > %OUT% || goto err
fc %OUT% helpp.txt || goto err

::много параметров 
%PROGRAM% 1 2 3 > %OUT% 
if not errorlevel 1 goto err
fc %OUT% Invalidmatrixf.txt || goto err

::файла input нет 
%PROGRAM% nofile.txt> %OUT%
if not errorlevel 1 goto err
fc %OUT% Invalidmatrixf.txt || goto err

::infile нет прав на чтение
set NOREAD="%TEMP%\in_noread.txt"
echo test > %NOREAD%

:: запрещаем чтение текущему пользователю
icacls %NOREAD% /inheritance:r >nul
icacls %NOREAD% /grant:r "%USERNAME%":W >nul

%PROGRAM% %NOREAD% > %OUT%
if not errorlevel 1 goto err
fc %OUT% Invalidmatrixf.txt || goto err

:: возвращаем нормальные права, чтобы можно было удалить
icacls %NOREAD% /reset >nul
del /f /q %NOREAD% >nul 2>nul


echo ALL tests passed
exit /B 0


:err
echo Program testing failed
exit /B 1
