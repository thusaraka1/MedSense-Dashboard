@echo off
echo ===================================================
echo   Starting MedSense Medical Dashboard System
echo ===================================================

echo.
echo [1/3] Starting Backend Server...
cd sensor-dashboard/backend
if not exist node_modules (
    echo    - Installing Backend Dependencies...
    call npm install
)
start "MedSense Backend" cmd /k "npm start"
cd ../..

echo.
echo [2/3] Starting Frontend Dashboard...
cd sensor-dashboard
if not exist node_modules (
    echo    - Installing Frontend Dependencies...
    call npm install
)
start "MedSense Frontend" cmd /k "npm run dev -- --host"
cd ..

echo.
echo [3/3] Opening Dashboard in Browser...
timeout /t 5
start http://localhost:5173

echo.
echo ===================================================
echo   System Started! 
echo   - Backend:  http://localhost:3001
echo   - Frontend: http://localhost:5173
echo ===================================================
pause
