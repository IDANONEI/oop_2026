@echo off

set PROGRAM="%~1"
set OUT="%TEMP%\out.txt"

::test1 - OK
%PROGRAM% 10 2 15 > %OUT% || goto err
fc %OUT% test1-out.txt || goto err

::test2 - OK
%PROGRAM% 2 10 1111 > %OUT% || goto err
fc %OUT% test2-out.txt || goto err

::test3 - invalid digit
%PROGRAM% 2 10 123 > %OUT%
if not errorlevel 1 goto err

::test4 - invalid source radix
%PROGRAM% 1 10 10 > %OUT%
if not errorlevel 1 goto err

::test5 - invalid destination radix
%PROGRAM% 10 37 10 > %OUT%
if not errorlevel 1 goto err

::test6 - too many parameters
%PROGRAM% 10 2 15 5 > %OUT%
if not errorlevel 1 goto err

::test7 - too few parameters
%PROGRAM% 10 2 > %OUT%
if not errorlevel 1 goto err

echo ALL tests passed
exit /B 0

:err
echo Program testing failed
exit /B 1