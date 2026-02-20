@echo off

set PROGRAM="%~1"
set OUT="%TEMP%\out.txt"

::test1 - OK (10 -> 2)
%PROGRAM% 10 2 15 > %OUT% || goto err
fc %OUT% test1-out.txt || goto err

::test2 - OK (2 -> 10)
%PROGRAM% 2 10 1111 > %OUT% || goto err
fc %OUT% test2-out.txt || goto err

::test3 - invalid digit
%PROGRAM% 2 10 123 > %OUT% 2>&1
if not errorlevel 1 goto err
findstr /C:"invalid digit" %OUT% >nul || goto err

::test4 - invalid source radix
%PROGRAM% 1 10 10 > %OUT% 2>&1
if not errorlevel 1 goto err
findstr /C:"invalid radix" %OUT% >nul || goto err

::test5 - invalid destination radix
%PROGRAM% 10 37 10 > %OUT% 2>&1
if not errorlevel 1 goto err
findstr /C:"invalid radix" %OUT% >nul || goto err

::test6 - too many parameters
%PROGRAM% 10 2 15 5 > %OUT% 2>&1
if not errorlevel 1 goto err

::test7 - too few parameters
%PROGRAM% 10 2 > %OUT% 2>&1
if not errorlevel 1 goto err

::test8 - OK (negative number)
%PROGRAM% 10 2 -15 > %OUT% || goto err
fc %OUT% test8-out.txt || goto err

::test9 - OK (INT_MIN in hex)
%PROGRAM% 10 16 -2147483648 > %OUT% || goto err
fc %OUT% test9-out.txt || goto err

::test10 - overflow (INT_MAX+1)
%PROGRAM% 10 10 2147483648 > %OUT% 2>&1
if not errorlevel 1 goto err
findstr /C:"overflow" %OUT% >nul || goto err

echo ALL tests passed
exit /B 0

:err
echo Program testing failed
type %OUT%
exit /B 1