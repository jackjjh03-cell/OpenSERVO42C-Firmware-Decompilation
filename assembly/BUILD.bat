@echo off
cd /d "%~dp0"
python build.py %*
exit /b %errorlevel%
