@echo off

if exist build\ (
	echo:
	echo Deleting Existing Build...
	echo:
	rd /s /q build\
	if exist build\ (
		echo:
		echo Failed to delete existing build files!
		echo:
		pause
		exit /b %errorlevel%
	)
)

call ReconfigureProject.bat
if %errorlevel% neq 0 (
	pause
	exit /b %errorlevel%
)

echo:
echo Building...
echo:
cd build
cmake --build . --config Debug

if %errorlevel% neq 0 (
	echo:
	echo Build failed!
	echo:
	pause
	exit /b %errorlevel%
)

start "" CPURasterizer.sln

echo:
echo Success!
pause