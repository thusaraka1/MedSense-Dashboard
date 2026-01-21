const express = require('express');
const { WebSocketServer } = require('ws');
const { SerialPort } = require('serialport');
const { ReadlineParser } = require('@serialport/parser-readline');
const cors = require('cors');

const app = express();
app.use(cors());

const PORT = 3001;
const WS_PORT = 3002;

// Store latest sensor data
let latestData = { bpm: 0, temp: 0, ir: 0 };

// HTTP endpoint to get latest data
app.get('/api/sensor-data', (req, res) => {
    res.json(latestData);
});

// Start HTTP server
app.listen(PORT, () => {
    console.log(`✓ HTTP Server running on http://localhost:${PORT}`);
});

// WebSocket server
const wss = new WebSocketServer({ port: WS_PORT });
console.log(`✓ WebSocket Server running on ws://localhost:${WS_PORT}`);

wss.on('connection', (ws) => {
    console.log('  → Client connected');

    // Send latest data immediately on connection
    ws.send(JSON.stringify(latestData));

    ws.on('close', () => {
        console.log('  ← Client disconnected');
    });
});

// Broadcast to all connected WebSocket clients
function broadcast(data) {
    wss.clients.forEach((client) => {
        if (client.readyState === 1) { // OPEN state
            client.send(JSON.stringify(data));
        }
    });
}

// Serial Port Configuration
// CHANGE THIS to match your ESP32 COM port (Windows: COM3, COM4, etc.)
const SERIAL_PORT = 'COM4'; // ← UPDATE THIS
const BAUD_RATE = 115200;

console.log(`\nAttempting to connect to ESP32 on ${SERIAL_PORT}...`);

const port = new SerialPort({
    path: SERIAL_PORT,
    baudRate: BAUD_RATE,
});

const parser = port.pipe(new ReadlineParser({ delimiter: '\n' }));

port.on('open', () => {
    console.log(`✓ Serial Port ${SERIAL_PORT} opened successfully\n`);
    console.log('Waiting for sensor data...\n');
});

port.on('error', (err) => {
    console.error(`✗ Serial Port Error: ${err.message}`);
    console.log('\nTroubleshooting:');
    console.log('1. Check if ESP32 is connected via USB');
    console.log('2. Update SERIAL_PORT in server.js (currently: ' + SERIAL_PORT + ')');
    console.log('3. Close Arduino IDE Serial Monitor if open\n');
});

parser.on('data', (line) => {
    try {
        // Only parse JSON lines
        if (line.trim().startsWith('{') && line.trim().endsWith('}')) {
            const data = JSON.parse(line.trim());

            // Validate data structure
            if (data.bpm !== undefined || data.temp !== undefined) {
                latestData = { ...latestData, ...data };
                console.log('📡', latestData);

                // Broadcast to all connected clients
                broadcast(latestData);
            }
        }
    } catch (e) {
        // Ignore parse errors (non-JSON lines from ESP32)
    }
});

// Graceful shutdown
process.on('SIGINT', () => {
    console.log('\nShutting down...');
    port.close();
    process.exit(0);
});
