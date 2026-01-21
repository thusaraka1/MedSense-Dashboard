import React from 'react';
import { Activity } from 'lucide-react';

const Header = () => {
    return (
        <header style={{
            display: 'flex',
            justifyContent: 'space-between',
            alignItems: 'center',
            padding: 'var(--spacing-md) var(--spacing-xl)',
            backgroundColor: 'var(--color-surface)',
            borderBottom: '1px solid var(--color-border)',
            boxShadow: 'var(--shadow-sm)',
            position: 'sticky',
            top: 0,
            zIndex: 100
        }}>
            <div style={{ display: 'flex', alignItems: 'center', gap: 'var(--spacing-sm)' }}>
                <div style={{
                    backgroundColor: 'var(--color-primary-light)',
                    padding: '8px',
                    borderRadius: '50%',
                    display: 'flex',
                    alignItems: 'center',
                    justifyContent: 'center'
                }}>
                    <Activity size={24} color="var(--color-primary)" />
                </div>
                <div>
                    <h1 style={{ fontSize: '1.25rem', color: 'var(--color-primary)', margin: 0 }}>MedSense Monitor</h1>
                    <p style={{ fontSize: '0.875rem', color: 'var(--color-text-secondary)', margin: 0 }}>Patient Vital Signs</p>
                </div>
            </div>

            <div style={{ display: 'flex', alignItems: 'center', gap: 'var(--spacing-sm)' }}>
                <span style={{
                    height: '10px',
                    width: '10px',
                    backgroundColor: 'var(--color-success)',
                    borderRadius: '50%',
                    display: 'inline-block',
                    boxShadow: '0 0 0 4px rgba(52, 199, 89, 0.2)'
                }} className="animate-pulse-slow"></span>
                <span style={{ fontSize: '0.875rem', fontWeight: 500, color: 'var(--color-success)' }}>
                    Live Monitoring
                </span>
            </div>
        </header>
    );
};

export default Header;
