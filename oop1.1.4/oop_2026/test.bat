@echo off

set PROGRAM="%~1"
set OUT="%TEMP%\out.txt"

::infile - OK по тз
%PROGRAM% < test1.txt > %OUT% || goto err
fc %OUT% test1-out.txt || goto err

::Пустой файл
%PROGRAM% < test2.txt > %OUT% || goto err
fc %OUT% test2-out.txt || goto err

::Тест 12312312345 - ктоторый точно надо проверить
%PROGRAM% < test3.txt > %OUT% || goto err
fc %OUT% test3-out.txt || goto err

::Пустой search
%PROGRAM% < test4.txt > %OUT% || goto err
fc %OUT% test4-out.txt || goto err

::help
%PROGRAM% < test5.txt > %OUT% || goto err
fc %OUT% test5-out.txt || goto err

::пустой line
%PROGRAM% < test6.txt > %OUT% || goto err
fc %OUT% test6-out.txt || goto err


::infile == outfile
%PROGRAM% in1.txt in1.txt ma mama > %OUT%
if not errorlevel 1 goto err
fc %OUT% error.txt || goto err

::infile - OK по тз
%PROGRAM% in1.txt out1.txt ma mama > %OUT% || goto err
fc out1.txt test1-out.txt || goto err

::help
%PROGRAM% -h > %OUT% || goto err
fc %OUT% test5-out.txt || goto err

::файла input нет 
%PROGRAM% nofile.txt outx.txt ma mama > %OUT%
if not errorlevel 1 goto err
fc %OUT% error.txt || goto err

::нельзя изменять outfile 
%PROGRAM% in1.txt C:\Windows\System32\out.txt ma mama > %OUT%
if not errorlevel 1 goto err
fc %OUT% error.txt || goto err


::infile нет прав на чтение
set NOREAD="%TEMP%\in_noread.txt"
echo test > %NOREAD%

:: запрещаем чтение текущему пользователю
icacls %NOREAD% /inheritance:r >nul
icacls %NOREAD% /grant:r "%USERNAME%":W >nul

%PROGRAM% %NOREAD% outx.txt ma mama > %OUT%
if not errorlevel 1 goto err
fc %OUT% error.txt || goto err

:: возвращаем нормальные права, чтобы можно было удалить
icacls %NOREAD% /reset >nul
del /f /q %NOREAD% >nul 2>nul



::файла out нет
%PROGRAM% in1.txt out1.txt ma > %OUT%
if not errorlevel 1 goto err
fc %OUT% error.txt || goto err

echo ALL tests passed
exit /B 0


:err
echo Program testing failed
exit /B 1
