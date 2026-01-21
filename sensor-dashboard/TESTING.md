# Quick Test Guide

## Step 1: Upload Arduino Code
1. Open Arduino IDE
2. Open: `d:/thilina/sensor-dashboard/arduino/combined_sensors/combined_sensors.ino`
3. Select your ESP32 board and COM port
4. Click Upload
5. Wait for "Done uploading"
6. **Note your COM port** (e.g., COM3, COM4) - you'll need this!

## Step 2: Configure Backend
1. Open: `d:/thilina/sensor-dashboard/backend/server.js`
2. Find line 50: `const SERIAL_PORT = 'COM3';`
3. Change `COM3` to YOUR ESP32's COM port
4. Save the file

## Step 3: Start Backend Server
Open a NEW terminal:
```bash
cd d:\thilina\sensor-dashboard\backend
npm start
```

Expected output:
```
✓ HTTP Server running on http://localhost:3001
✓ WebSocket Server running on ws://localhost:3002
✓ Serial Port COM3 opened successfully
```

You should see data flowing:
```
📡 { bpm: 0, temp: 25.3, ir: 1234 }
📡 { bpm: 0, temp: 25.4, ir: 1567 }
```

## Step 4: Open Dashboard
1. Frontend is already running at: http://localhost:5173
2. Open it in your browser
3. Click **"Connect to Backend"** button

## Step 5: Test Temperature Sensor
1. **Without finger**: Should show ~25°C and "Ambient Temperature"
2. **Place finger on MLX90614**: Watch temperature smoothly increase
3. **Final reading**: Should reach ~36-37°C and show "Body Temperature - Normal"

Both OLED and web should show the same values!

## Troubleshooting
- **Backend won't start**: Close Arduino Serial Monitor first
- **No data**: Check COM port in server.js matches ESP32
- **Can't connect**: Make sure backend is running before clicking connect
