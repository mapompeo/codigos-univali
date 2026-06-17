@echo off
setlocal

REM ============================================================
REM  build.bat - compila e executa o Mercadinho
REM
REM  PRE-REQUISITOS (instalar uma vez):
REM    1. w64devkit  -> https://github.com/skeeto/w64devkit/releases
REM       Extraia para C:\w64devkit
REM    2. Raylib (pacote MinGW-w64) ->
REM       https://github.com/raysan5/raylib/releases
REM       Baixe raylib-X.X.X_win64_mingw-w64.zip
REM       Extraia para C:\raylib
REM       (deve conter C:\raylib\include\raylib.h e C:\raylib\lib\libraylib.a)
REM ============================================================

REM --- caminhos (ajuste se instalou em outro lugar) -----------
set W64=C:\w64devkit\bin
set RAYLIB_INC=C:\raylib\include
set RAYLIB_LIB=C:\raylib\lib

REM --- verifica se gcc existe ---------------------------------
if not exist "%W64%\gcc.exe" (
    echo [ERRO] gcc nao encontrado em %W64%
    echo Instale o w64devkit em C:\w64devkit
    echo Download: https://github.com/skeeto/w64devkit/releases
    pause
    exit /b 1
)

REM --- verifica raylib.h --------------------------------------
if not exist "%RAYLIB_INC%\raylib.h" (
    echo [ERRO] raylib.h nao encontrado em %RAYLIB_INC%
    echo Baixe o pacote MinGW-w64 do Raylib e extraia em C:\raylib
    echo Download: https://github.com/raysan5/raylib/releases
    pause
    exit /b 1
)

REM --- cria pasta data se nao existir -------------------------
if not exist data mkdir data

echo [INFO] Compilando Mercadinho...

set SRCS=src\main.c src\hash.c src\fila.c src\pilha.c src\bst.c src\sort.c src\dados.c src\ui.c src\tela_menu.c src\tela_cadastro.c src\tela_caixa.c src\tela_relatorios.c src\tela_auditoria.c

"%W64%\gcc.exe" -std=c99 -Wall -O2 ^
    -I"%RAYLIB_INC%" ^
    -o mercadinho.exe ^
    %SRCS% ^
    -L"%RAYLIB_LIB%" -lraylib -lopengl32 -lgdi32 -lwinmm -mwindows

if %errorlevel% neq 0 (
    echo [ERRO] Compilacao falhou! Verifique os erros acima.
    pause
    exit /b 1
)

echo [OK] Compilacao bem-sucedida!
echo [INFO] Iniciando Mercadinho...
start "" mercadinho.exe

endlocal
