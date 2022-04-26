@ECHO off

IF NOT EXIST bin MKDIR bin

IF "%~1"=="clean" GOTO clean_dir
IF "%~1"=="server" GOTO run_server
IF "%~1"=="setup" GOTO run_setup

IF NOT EXIST bin\lua.o GOTO build_lua
GOTO build_app

:clean_dir
ECHO cleaning build...
IF EXIST "bin" DEL bin\*.*?
GOTO end

:run_server
ECHO starting server...
PUSHD bin
START python -m http.server
POPD
GOTO end

:run_setup
ECHO setting up emsdk...
PUSHD app\emsdk
CALL emsdk install latest
CALL emsdk activate latest
CALL emsdk_env.bat
POPD
GOTO end

:build_lua
ECHO building lua library...
PUSHD app\lua
CALL emcc -c -DMAKE_LIB onelua.c -o ..\..\bin\lua.o
POPD
GOTO build_app

:build_app
ECHO building application...
PUSHD bin
CALL emcc -c -I ..\app\lua ..\app\pixapp.c -o pixapp.o
CALL emcc -s WASM=1 -s USE_SDL=2 lua.o pixapp.o -o pixtoy.js
COPY ..\web\*.html *.html
COPY ..\web\*.js *.js
COPY ..\web\ace\*.js *.js
COPY ..\web\*.css *.css
POPD
GOTO end

:end
ECHO complete!
