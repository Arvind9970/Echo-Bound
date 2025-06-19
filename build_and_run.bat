@echo off
echo Compiling...

echo Compiling simple_listener...
gcc -o listener.exe simple_listener.c -lws2_32
if %ERRORLEVEL% NEQ 0 (
    echo Failed to compile listener
    pause
    exit /b 1
)

echo Compiling simple_client...
gcc -o client.exe simple_client.c -lws2_32
if %ERRORLEVEL% NEQ 0 (
    echo Failed to compile client
    pause
    exit /b 1
)

echo.
echo =======================================
echo 1. Run the listener in one terminal:
echo    listener.exe
echo.
echo 2. Then run the client in another terminal:
echo    client.exe
echo.
echo 3. Type messages in the client and see them
echo    echoed back by the server.
echo =======================================
echo.
pause
