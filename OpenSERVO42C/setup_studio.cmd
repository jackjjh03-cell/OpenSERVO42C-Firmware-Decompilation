@echo off
setlocal
pushd "%~dp0" || exit /b 1
set "PY=py -3.8"
%PY% -c "import sys; sys.exit(sys.version_info < (3,8))" >nul 2>&1
if errorlevel 1 set "PY=python"
%PY% -c "import sys, tkinter; sys.exit(sys.version_info < (3,8))" >nul 2>&1
if errorlevel 1 (
    echo Install Python 3.8 or newer with Tcl/Tk before running setup.
    echo On Windows 7 use Python 3.8.
    goto failed
)
%PY% -m pip install --user -r studio\requirements.txt
if errorlevel 1 goto failed
%PY% -c "import serial, tkinter; print('Studio dependencies available.')"
if errorlevel 1 goto failed
echo Setup complete. Start Studio with start_studio.cmd.
pause
popd
exit /b 0
:failed
echo Setup failed. Review the Python or pip message above.
pause
popd
exit /b 1
