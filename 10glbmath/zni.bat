@echo off
set /a JOB_NUM=%NUMBER_OF_PROCESSORS%*6/5

echo compile ...
echo.

if exist obj (
  call del /s /F /Q /S obj 1>nul
  call rmdir /Q /S obj
)

call ndk-build clean
call ndk-build -j %JOB_NUM% -B

if errorlevel 1 (
    echo.
    echo EEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEE
    echo E
    echo E exit with build error level :: %errorlevel%
    echo E
    echo EEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEE
    echo.
    exit /b %errorlevel%
)

echo.
echo ********************************************************************************
echo ** library copy ...
echo ********************************************************************************
echo.

call xcopy /C/Q/R/Y obj\local\armeabi\*.a ..\lib\
