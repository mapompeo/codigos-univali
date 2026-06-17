@echo off
setlocal
echo ============================================================
echo  INSTALADOR AUTOMATICO - Mercadinho (Raylib + C)
echo ============================================================
echo.

REM --- verifica winget ----------------------------------------
where winget >nul 2>&1
if %errorlevel% neq 0 (
    echo [AVISO] winget nao disponivel. Instale manualmente:
    echo   1. w64devkit: https://github.com/skeeto/w64devkit/releases
    echo   2. Raylib:    https://github.com/raysan5/raylib/releases
    goto :manual
)

REM --- baixa w64devkit via winget (se nao instalado) ----------
if not exist "C:\w64devkit\bin\gcc.exe" (
    echo [INFO] Baixando w64devkit (compilador C para Windows)...
    winget install --id skeeto.w64devkit -e --silent
    if %errorlevel% neq 0 (
        echo [AVISO] winget nao encontrou w64devkit. Baixe manualmente:
        echo   https://github.com/skeeto/w64devkit/releases
        echo   Extraia para C:\w64devkit
    )
) else (
    echo [OK] w64devkit ja instalado.
)

REM --- baixa Raylib (pacote pre-compilado MinGW) ---------------
if not exist "C:\raylib\include\raylib.h" (
    echo.
    echo [INFO] Raylib nao encontrado em C:\raylib
    echo.
    echo  Por favor, baixe o pacote manualmente:
    echo  1. Acesse: https://github.com/raysan5/raylib/releases
    echo  2. Baixe o arquivo: raylib-X.X.X_win64_mingw-w64.zip
    echo  3. Extraia e copie o conteudo para C:\raylib
    echo     (deve conter: C:\raylib\include\raylib.h)
    echo                   C:\raylib\lib\libraylib.a
    echo.
    echo  Depois de extrair, execute build.bat novamente.
    echo.
    pause
    exit /b 1
) else (
    echo [OK] Raylib ja encontrado em C:\raylib
)

echo.
echo [INFO] Tudo instalado! Compilando o projeto...
echo.
call build.bat
goto :fim

:manual
echo.
echo Apos instalar w64devkit e Raylib, execute build.bat
pause

:fim
endlocal
