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