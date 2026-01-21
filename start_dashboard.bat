@echo off
echo ===================================================
echo   Starting MedSense Medical Dashboard System
echo ===================================================

echo.
echo [1/3] Starting Backend Server...
start "MedSense Backend" cmd /k "cd sensor-dashboard/backend && npm start"

echo.
echo [2/3] Starting Frontend Dashboard...
start "MedSense Frontend" cmd /k "cd sensor-dashboard && npm run dev -- --host"

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
