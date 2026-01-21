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

const BAUD_RATE = 115200;
let port;

async function startSerialConnection() {
    try {
        const ports = await SerialPort.list();
        console.log('Available Ports:', ports.map(p => `${p.path} (${p.manufacturer || 'unknown'})`).join(', '));

        // Auto-detect strategy: Look for common ESP32/Arduino USB drivers
        // CP210x (Silicon Labs), CH340 (wch.cn / Qinheng), FTDI
        const espPort = ports.find(p =>
            p.manufacturer && (
                p.manufacturer.includes('Silicon') ||
                p.manufacturer.includes('wch') ||
                p.manufacturer.includes('Qinheng') ||
                p.manufacturer.includes('Prolific') ||
                p.manufacturer.includes('Arduino')
            )
        ) || ports[0]; // Fallback to first available port if no match

        if (!espPort) {
            console.error('✗ No Serial Ports found! Connect your ESP32 via USB.');
            setTimeout(startSerialConnection, 5000); // Retry in 5s
            return;
        }

        console.log(`\nAttempting to connect to auto-detected port: ${espPort.path}...`);

        port = new SerialPort({
            path: espPort.path,
            baudRate: BAUD_RATE,
            autoOpen: false
        });

        const parser = port.pipe(new ReadlineParser({ delimiter: '\n' }));

        port.open((err) => {
            if (err) {
                console.error(`✗ Failed to open ${espPort.path}: ${err.message}`);
                console.log('  (Retrying in 5 seconds...)');
                setTimeout(startSerialConnection, 5000);
            }
        });

        port.on('open', () => {
            console.log(`✓ Serial Port ${espPort.path} opened successfully\n`);
            console.log('Waiting for sensor data...\n');
        });

        port.on('close', () => {
            console.log('! Serial Port Disconnected. Reconnecting...');
            setTimeout(startSerialConnection, 3000);
        });

        port.on('error', (err) => {
            console.error(`✗ Serial Port Error: ${err.message}`);
        });

        // Data Handler
        parser.on('data', (line) => {
            // ... existing data logic ...
            handleData(line);
        });

    } catch (error) {
        console.error('Error listing ports:', error);
        setTimeout(startSerialConnection, 5000);
    }
}

// Start the connection process
startSerialConnection();


function handleData(line) {
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
        // Ignore parse errors
    }
}

// Graceful shutdown
process.on('SIGINT', () => {
    console.log('\nShutting down...');
    if (port && port.isOpen) {
        port.close();
    }
    process.exit(0);
});
