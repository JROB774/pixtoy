@ECHO off

IF NOT EXIST web\app MKDIR web\app

IF "%~1"=="clean" GOTO clean_dir
IF "%~1"=="server" GOTO run_server
IF "%~1"=="setup" GOTO run_setup

GOTO build_app

:clean_dir
ECHO cleaning build...
IF EXIST "web\app" DEL web\app\*.*?
GOTO end

:run_server
ECHO starting server...
PUSHD web
START python -m http.server
POPD
GOTO end

:run_setup
ECHO setting up emsdk...
CALL app\emsdk\emsdk install latest
CALL app\emsdk\emsdk activate latest
CALL app\emsdk\emsdk_env.bat
GOTO end

:build_app
ECHO building application...
IF NOT EXIST web\app MKDIR web\app
CALL emcc -s WASM=1 -s USE_SDL=2 -I app\lua app\pixweb.c -o web\app\pixtoy.js
GOTO end

:end
ECHO complete!
