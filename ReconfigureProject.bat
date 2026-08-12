@echo off

echo:
echo Configuring...
echo:
cmake -S . -B build

:: check for errors with configuration
if %errorlevel% neq 0 (
	echo:
	echo Configuration failed!
	echo:
	pause
	exit /b %errorlevel%
)

exit /b 0