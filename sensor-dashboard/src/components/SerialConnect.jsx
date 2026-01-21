import React, { useState } from 'react';
import { Plug, PlugZap } from 'lucide-react';

const SerialConnect = ({ onDataReceived }) => {
    const [isConnected, setIsConnected] = useState(false);
    const [error, setError] = useState(null);

    const connectSerial = async () => {
        try {
            if (!navigator.serial) {
                setError("Web Serial API not supported in this browser.");
                return;
            }

            const port = await navigator.serial.requestPort();
            await port.open({ baudRate: 115200 });
            setIsConnected(true);
            setError(null);

            const textDecoder = new TextDecoderStream();
            const readableStreamClosed = port.readable.pipeTo(textDecoder.writable);
            const reader = textDecoder.readable.getReader();

            let buffer = '';

            try {
                while (true) {
                    const { value, done } = await reader.read();
                    if (done) break;

                    buffer += value;
                    const lines = buffer.split('\n');
                    buffer = lines.pop(); // Keep incomplete line in buffer

                    for (const line of lines) {
                        try {
                            if (line.trim().startsWith('{') && line.trim().endsWith('}')) {
                                const json = JSON.parse(line.trim());
                                onDataReceived(json);
                            }
                        } catch (e) {
                            // Ignore parse errors from noise
                            console.log('Parse error:', e);
                        }
                    }
                }
            } catch (e) {
                console.error(e);
                setError("Connection lost");
                setIsConnected(false);
            } finally {
                reader.releaseLock();
            }
        } catch (e) {
            console.error(e);
            setError("Failed to connect");
        }
    };

    return (
        <div style={{ marginBottom: 'var(--spacing-md)' }}>
            <button
                onClick={connectSerial}
                disabled={isConnected}
                style={{
                    display: 'flex',
                    alignItems: 'center',
                    gap: '8px',
                    backgroundColor: isConnected ? 'var(--color-success)' : 'var(--color-primary)',
                    color: '#fff',
                    padding: '10px 20px',
                    border: 'none',
                    borderRadius: 'var(--radius-sm)',
                    cursor: isConnected ? 'default' : 'pointer',
                    opacity: isConnected ? 0.9 : 1
                }}
            >
                {isConnected ? <PlugZap size={20} /> : <Plug size={20} />}
                {isConnected ? 'Device Connected' : 'Connect Device'}
            </button>
            {error && <p style={{ color: 'var(--color-danger)', marginTop: '8px', fontSize: '0.9rem' }}>{error}</p>}
        </div>
    );
};

export default SerialConnect;
