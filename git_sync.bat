@echo off
echo ===================================================
echo   Syncing MedSense Dashboard to GitHub
echo ===================================================

echo.
echo [1/3] Adding all changes...
git add .

echo.
echo [2/3] Committing changes...
set /p commit_msg="Enter commit message (Press Enter for 'Auto Update'): "
if "%commit_msg%"=="" set commit_msg=Auto Update %date% %time%
git commit -m "%commit_msg%"

echo.
echo [3/3] Pushing to GitHub...
git push

echo.
echo ===================================================
echo   Sync Complete! 🐙
echo ===================================================
pause
