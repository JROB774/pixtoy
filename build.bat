@ECHO off
SETLOCAL

IF NOT EXIST "binary" MKDIR "binary"
emcc source/main.c -o binary/pixtoy.html

ENDLOCAL
