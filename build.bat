@ECHO off

IF NOT EXIST binary MKDIR binary

IF "%~1"=="clean" GOTO clean_dir
IF "%~1"=="server" GOTO run_server
IF "%~1"=="setup" GOTO run_setup

IF NOT EXIST binary\lua.o GOTO build_lua
GOTO build_app

:clean_dir
ECHO cleaning build...
IF EXIST "binary" DEL binary\*.*?
GOTO end

:run_server
ECHO starting server...
PUSHD binary
START python -m http.server
POPD
GOTO end

:run_setup
ECHO setting up emsdk...
PUSHD emsdk
CALL emsdk install latest
CALL emsdk activate latest
CALL emsdk_env.bat
POPD
GOTO end

:build_lua
ECHO building lua library...
PUSHD lua
CALL emcc -c -DMAKE_LIB onelua.c -o ..\binary\lua.o
POPD
GOTO build_app

:build_app
ECHO building application...
PUSHD binary
CALL emcc -c -I ..\lua ..\source\main.c -o pixtoy.o
CALL emcc -s USE_SDL=2 lua.o pixtoy.o -o pixtoy.html
COPY ..\source\*.html *.html
COPY ..\source\*.js *.js
COPY ..\source\*.css *.css
POPD
GOTO end

:end
ECHO complete!
