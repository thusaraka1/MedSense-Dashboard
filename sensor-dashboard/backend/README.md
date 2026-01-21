# Sensor Dashboard - Backend Server

This Node.js backend bridges your ESP32 sensor data to the web frontend via WebSocket.

## Quick Start

### 1. Install Dependencies
```bash
cd backend
npm install
```

### 2. Configure Serial Port
Edit `server.js` and update the COM port:
```javascript
const SERIAL_PORT = 'COM3'; // ← Change to your ESP32's port
```

**Windows**: Check Device Manager → Ports (COM & LPT)
**Find the port**: Usually COM3, COM4, COM5, etc.

### 3. Upload Arduino Code
1. Open `arduino/combined_sensors/combined_sensors.ino` in Arduino IDE
2. Upload to your ESP32
3. **Close Arduino Serial Monitor** (backend needs exclusive access)

### 4. Start Backend Server
```bash
npm start
```

You should see:
```
✓ HTTP Server running on http://localhost:3001
✓ WebSocket Server running on ws://localhost:3002
✓ Serial Port COM3 opened successfully
```

### 5. Start Frontend
In a new terminal:
```bash
cd ..
npm run dev
```

### 6. Connect
1. Open http://localhost:5173 in your browser
2. Click **"Connect to Backend"** button
3. Live data should appear!

## Troubleshooting

### "Serial Port Error: Access denied"
- Close Arduino IDE Serial Monitor
- Restart the backend

### "Cannot find COM port"
- Check Device Manager for correct port
- Update `SERIAL_PORT` in server.js

### "WebSocket connection failed"
- Make sure backend is running (`npm start` in backend folder)
- Check if port 3002 is available
