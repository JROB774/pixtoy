@ECHO off
SETLOCAL

IF NOT EXIST "binary" MKDIR "binary"
PUSHD binary
START python -m http.server
POPD

ENDLOCAL
