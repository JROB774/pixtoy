@echo off

if not exist web\app mkdir web\app

if "%~1"=="clean" goto clean_dir
if "%~1"=="server" goto run_server
if "%~1"=="setup" goto run_setup

goto build_app

:clean_dir
echo cleaning build...
if exist "web\app" del web\app\*.*?
goto end

:run_server
echo starting server...
pushd web
start python -m http.server
popd
goto end

:run_setup
echo setting up emsdk...
call app\emsdk\emsdk install latest
call app\emsdk\emsdk activate latest
call app\emsdk\emsdk_env.bat
goto end

:build_app
echo building application...
call emcc -s EXPORTED_FUNCTIONS=_main,_app_build,_app_reset,_app_start,_app_video -s WASM=1 -s USE_SDL=2 -I app\lua app\pixweb.c -o web\app\pixtoy.js
goto end

:end
echo complete!
