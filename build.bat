@ECHO off
SETLOCAL

IF "%~1"=="clean" GOTO clean_dir

IF NOT EXIST binary MKDIR binary
IF NOT EXIST binary\lua.o GOTO build_lua
GOTO build_app

:clean_dir
ECHO cleaning build...
IF EXIST "binary" DEL binary\*.*?
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
CALL emcc lua.o pixtoy.o -o pixtoy.html
POPD
GOTO end

:end
ECHO complete!
ENDLOCAL
