# Medical Sensor Dashboard

Real-time medical sensor data visualization with ESP32, MAX30102 (Heart Rate), and MLX90614 (Temperature).

## 🚀 Quick Start

### 1. Upload ESP32 Firmware
```bash
# Open in Arduino IDE
d:/thilina/sensor-dashboard/arduino/combined_sensors/combined_sensors.ino

# Install libraries: MAX30105, Adafruit MLX90614, U8g2
# Upload to ESP32
# Note your COM port!
```

### 2. Start Backend Server
```bash
cd backend

# Edit server.js line 52 to match your COM port
# Then:
npm start
```

### 3. Start Frontend (if not running)
```bash
npm run dev
```

### 4. Open Dashboard
- Navigate to http://localhost:5173
- Click "Connect to Backend"
- Watch live data flow!

## 📊 What to Expect

### Temperature Sensor (MLX90614)
- **No finger**: ~25°C - "Ambient Temperature"  
- **Finger placed**: Smoothly rises to ~37°C - "Body Temperature - Normal"
- **Fever**: >38°C - "High Temperature!"

### Heart Rate Sensor (MAX30102)
- Place finger on sensor
- Wait 3-5 seconds for reading to stabilize
- Normal range: 60-100 BPM

## 🛠️ Troubleshooting

**Backend won't start?**
- Close Arduino Serial Monitor
- Check COM port in `backend/server.js`

**No data on dashboard?**
- Ensure backend shows: `📡 { bpm: X, temp: Y }`
- Click "Connect to Backend" button

**Different values on OLED vs Web?**
- Re-upload Arduino code (both now synchronized)

## 📁 Project Structure

```
sensor-dashboard/
├── arduino/combined_sensors/   # ESP32 firmware
├── backend/                     # Node.js Serial→WebSocket bridge
│   ├── server.js               # Main server (update COM port here!)
│   └── package.json
└── src/                        # React frontend
    └── components/
        ├── Dashboard.jsx       # Main UI
        ├── WebSocketConnect.jsx
        └── SensorCard.jsx
```

## 🎨 Medical Theme Features
- Clean, clinical design
- Color-coded alerts (green/yellow/red)
- Real-time status messages
- Responsive layout

---

**Created by**: Antigravity AI  
**Hardware**: ESP32 + MAX30102 + MLX90614 + SH1106 OLED
