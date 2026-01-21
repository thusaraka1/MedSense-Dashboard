import React, { useState, useEffect } from 'react';
import { Plug, PlugZap } from 'lucide-react';

const WebSocketConnect = ({ onDataReceived }) => {
    const [isConnected, setIsConnected] = useState(false);
    const [error, setError] = useState(null);
    const [ws, setWs] = useState(null);

    const connectWebSocket = () => {
        try {
            const websocket = new WebSocket('ws://localhost:3002');

            websocket.onopen = () => {
                console.log('WebSocket connected');
                setIsConnected(true);
                setError(null);
            };

            websocket.onmessage = (event) => {
                try {
                    const data = JSON.parse(event.data);
                    onDataReceived(data);
                } catch (e) {
                    console.error('Parse error:', e);
                }
            };

            websocket.onerror = (error) => {
                console.error('WebSocket error:', error);
                setError('Connection error. Is the backend server running?');
            };

            websocket.onclose = () => {
                console.log('WebSocket disconnected');
                setIsConnected(false);
                setError('Connection closed');
            };

            setWs(websocket);
        } catch (e) {
            console.error(e);
            setError('Failed to connect to backend server');
        }
    };

    const disconnect = () => {
        if (ws) {
            ws.close();
            setWs(null);
        }
    };

    return (
        <div style={{ marginBottom: 'var(--spacing-md)' }}>
            <button
                onClick={isConnected ? disconnect : connectWebSocket}
                style={{
                    display: 'flex',
                    alignItems: 'center',
                    gap: '8px',
                    backgroundColor: isConnected ? 'var(--color-success)' : 'var(--color-primary)',
                    color: '#fff',
                    padding: '10px 20px',
                    border: 'none',
                    borderRadius: 'var(--radius-sm)',
                    cursor: 'pointer',
                    opacity: isConnected ? 0.9 : 1
                }}
            >
                {isConnected ? <PlugZap size={20} /> : <Plug size={20} />}
                {isConnected ? 'Connected (Click to Disconnect)' : 'Connect to Backend'}
            </button>
            {error && <p style={{ color: 'var(--color-danger)', marginTop: '8px', fontSize: '0.9rem' }}>{error}</p>}
        </div>
    );
};

export default WebSocketConnect;
