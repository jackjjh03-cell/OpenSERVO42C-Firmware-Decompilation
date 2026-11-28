@echo off
setlocal
pushd "%~dp0" || exit /b 1
set "PY=py -3.8"
%PY% -c "import sys; sys.exit(sys.version_info < (3,8))" >nul 2>&1
if errorlevel 1 set "PY=python"
%PY% -c "import sys; sys.exit(sys.version_info < (3,8))" >nul 2>&1
if errorlevel 1 (
    echo Python 3.8 or newer is required. On Windows 7 use Python 3.8.
    goto failed
)
%PY% -c "import tkinter, serial" >nul 2>&1
if errorlevel 1 (
    echo Python Tk or pyserial is missing. Include Tcl/Tk when installing Python
    echo and run setup_studio.cmd to install the serial dependency.
    goto failed
)
%PY% -m studio.app
if errorlevel 1 (
    echo Studio exited with an error. Review the message above.
    goto failed
)
popd
exit /b 0
:failed
pause
popd
exit /b 1
