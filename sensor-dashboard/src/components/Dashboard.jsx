import React, { useState, useEffect } from 'react';
import { Heart, Thermometer } from 'lucide-react';
import Header from './Header';
import SensorCard from './SensorCard';
import WebSocketConnect from './WebSocketConnect';
import IRGraph from './IRGraph';

const Dashboard = () => {
    const [heartRate, setHeartRate] = useState(0);
    const [temperature, setTemperature] = useState(0);
    const [irHistory, setIrHistory] = useState([]);
    const [isSimulating, setIsSimulating] = useState(true);

    // Simulation Logic (Only runs if isSimulating is true)
    useEffect(() => {
        if (!isSimulating) return;

        // Set initial values for simulation
        setHeartRate(72);
        setTemperature(36.6);

        const interval = setInterval(() => {
            setHeartRate(prev => {
                const change = Math.floor(Math.random() * 5) - 2;
                let newValue = prev + change;
                if (newValue < 60) newValue = 60;
                if (newValue > 120) newValue = 120;
                return newValue;
            });

            setTemperature(prev => {
                const change = (Math.random() * 0.2) - 0.1;
                let newValue = prev + change;
                return parseFloat(newValue.toFixed(1));
            });

        }, 2000);

        return () => clearInterval(interval);
    }, [isSimulating]);

    const handleSensorData = (data) => {
        if (isSimulating) setIsSimulating(false); // Stop simulation on first real data

        if (data.bpm !== undefined) setHeartRate(data.bpm);
        if (data.temp !== undefined) setTemperature(data.temp);

        if (data.ir !== undefined) {
            setIrHistory(prev => {
                const newHistory = [...prev, { index: Date.now(), value: data.ir }];
                if (newHistory.length > 50) return newHistory.slice(1);
                return newHistory;
            });
        }
    };

    const getHeartRateStatus = (val) => {
        if (val < 60 || val > 100) return 'warning';
        return 'normal';
    };

    const getTempStatus = (val) => {
        if (val < 35 || val > 38) return 'critical';
        if (val > 37.2) return 'warning';
        return 'normal';
    };

    return (
        <div style={{ flex: 1, display: 'flex', flexDirection: 'column' }}>
            <Header />

            <main style={{
                padding: 'var(--spacing-xl)',
                maxWidth: '1200px',
                width: '100%',
                margin: '0 auto',
                flex: 1
            }}>
                <div style={{
                    display: 'flex',
                    justifyContent: 'space-between',
                    alignItems: 'center',
                    marginBottom: 'var(--spacing-lg)'
                }}>
                    <div>
                        <h2 style={{ fontSize: '1.5rem', marginBottom: 'var(--spacing-xs)' }}>Overview</h2>
                        <p style={{ color: 'var(--color-text-secondary)' }}>
                            {isSimulating ? 'Simulated Data (Connect Device for Live Readings)' : 'Live Patient Data'}
                        </p>
                    </div>
                    <WebSocketConnect onDataReceived={handleSensorData} />
                </div>

                <div style={{
                    display: 'grid',
                    gridTemplateColumns: 'repeat(auto-fit, minmax(300px, 1fr))',
                    gap: 'var(--spacing-lg)'
                }}>
                    {/* Heart Rate Card */}
                    <SensorCard
                        title="Heart Rate"
                        value={heartRate}
                        unit="BPM"
                        icon={Heart}
                        color="#FF2D55"
                        status={getHeartRateStatus(heartRate)}
                    />

                    {/* Temperature Card */}
                    <SensorCard
                        title="Body Temperature"
                        value={temperature}
                        unit="°C"
                        icon={Thermometer}
                        color="#FF9F0A"
                        status={getTempStatus(temperature)}
                        isTemperature={true}
                    />
                </div>

                <div style={{ marginTop: 'var(--spacing-lg)' }}>
                    <IRGraph data={irHistory} />
                </div>
            </main>
        </div>
    );
};

export default Dashboard;
